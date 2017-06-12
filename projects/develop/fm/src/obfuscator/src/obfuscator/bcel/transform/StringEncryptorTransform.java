package obfuscator.bcel.transform;

import java.util.ArrayList;
import java.util.List;
import obfuscator.bcel.UseBCEL;

import org.apache.bcel.Constants;
import org.apache.bcel.classfile.ConstantString;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.ALOAD;
import org.apache.bcel.generic.ASTORE;
import org.apache.bcel.generic.ClassGen;
import org.apache.bcel.generic.ConstantPoolGen;
import org.apache.bcel.generic.DUP;
import org.apache.bcel.generic.GOTO;
import org.apache.bcel.generic.I2B;
import org.apache.bcel.generic.I2C;
import org.apache.bcel.generic.IAND;
import org.apache.bcel.generic.ICONST;
import org.apache.bcel.generic.IINC;
import org.apache.bcel.generic.ILOAD;
import org.apache.bcel.generic.ISTORE;
import org.apache.bcel.generic.IXOR;
import org.apache.bcel.generic.InstructionFactory;
import org.apache.bcel.generic.InstructionHandle;
import org.apache.bcel.generic.InstructionList;
import org.apache.bcel.generic.InvokeInstruction;
import org.apache.bcel.generic.LDC;
import org.apache.bcel.generic.MethodGen;
import org.apache.bcel.generic.ObjectType;
import org.apache.bcel.generic.POP;
import org.apache.bcel.generic.SIPUSH;
import org.apache.bcel.generic.Type;

/**
 * Encrypts strings. 
 */
public class StringEncryptorTransform extends ObTransform {
	private String methodName;
	private int callsChanged;
	private List<String> encryptedStrings;

	public StringEncryptorTransform(final ClassGen cg) {
		super(cg);
		methodName = "aA";
		while (cg.containsMethod(methodName, Type.getMethodSignature(
				Type.STRING, new Type[] { Type.STRING })) != null) {
			methodName = methodName + "A";
		}
		encryptedStrings = new ArrayList<String>();
		System.out.printf(
				"Loading encryptor for %s, decrypt method name: %s\n",
				cg.getClassName(), methodName);
	}

	public void execute() {
		UseBCEL.println("Starting encryption on class " + cg.getClassName());
		changeCalls();
		UseBCEL.println("\tChanged " + callsChanged + " LDCs");
		if (callsChanged > 0) {
			insertMethod();
			UseBCEL.println("\tInserted method " + methodName);
		}
	}

	private void insertMethod() {
		// this method generates the decryption method, this way I could
		// technically encrypt strings with information from the classes
		// themselves, making it harder to decrypt
		InstructionList list = new InstructionList();
		MethodGen newMethod = new MethodGen(
				(Constants.ACC_PRIVATE | Constants.ACC_STATIC), Type.STRING,
				new Type[] { Type.STRING }, new String[] { "encrypted" },
				methodName, cg.getClassName(), list, cg.getConstantPool());
		ConstantPoolGen cpg = cg.getConstantPool();
		InstructionFactory factory = new InstructionFactory(cpg);
		list.append(factory.createNew((ObjectType) Type
				.getType(StringBuilder.class)));
		list.append(new DUP());
		list.append(factory.createInvoke("java.lang.StringBuilder", "<init>",
				Type.VOID, new Type[] {}, Constants.INVOKESPECIAL));
		list.append(new ASTORE(1));
		list.append(new ALOAD(0));
		list.append(factory.createInvoke("java.lang.String", "length",
				Type.INT, new Type[] {}, Constants.INVOKEVIRTUAL));
		list.append(new SIPUSH((short) 255));
		list.append(new IAND());
		list.append(new I2B());
		list.append(new ISTORE(2));
		list.append(new ICONST(0));
		InstructionHandle go1Prev = list.append(new ISTORE(3));
		InstructionHandle go1After = list.append(new ALOAD(1));
		list.append(new ALOAD(0));
		list.append(new ILOAD(3));
		list.append(factory.createInvoke("java.lang.String", "charAt",
				Type.CHAR, new Type[] { Type.INT }, Constants.INVOKEVIRTUAL));
		list.append(new ILOAD(2));
		list.append(new IXOR());
		list.append(new I2C());
		list.append(factory.createInvoke("java.lang.StringBuilder", "append",
				Type.getType("Ljava/lang/StringBuilder;"),
				new Type[] { Type.CHAR }, Constants.INVOKEVIRTUAL));
		list.append(new POP());
		list.append(new ILOAD(2));
		list.append(new ILOAD(3));
		list.append(new IXOR());
		list.append(new I2B());
		list.append(new ISTORE(2));
		list.append(new IINC(3, 1));
		list.append(go1Prev, new GOTO(list.append(new ILOAD(3))));
		list.append(new ALOAD(0));
		list.append(factory.createInvoke("java.lang.String", "length",
				Type.INT, new Type[] {}, Constants.INVOKEVIRTUAL));
		list.append(InstructionFactory.createBranchInstruction(
				Constants.IF_ICMPLT, go1After));
		list.append(new ALOAD(1));
		list.append(factory.createInvoke("java.lang.StringBuilder", "toString",
				Type.STRING, new Type[] {}, Constants.INVOKEVIRTUAL));
		list.append(factory.createInvoke("java.lang.String", "intern",
				Type.STRING, new Type[] {}, Constants.INVOKEVIRTUAL));
		list.append(InstructionFactory.createReturn(Type.STRING));
		newMethod.setMaxLocals();
		newMethod.setMaxStack();
		cg.addMethod(newMethod.getMethod());
	}

	private void changeCalls() {
		InvokeInstruction invoke = new InstructionFactory(cg).createInvoke(
				cg.getClassName(), methodName, Type.STRING,
				new Type[] { Type.STRING }, Constants.INVOKESTATIC);
                boolean isChanged;
		for (Method m : cg.getMethods()) {
			if (m.isAbstract() || m.isNative()) {
				continue;
                        }
			ConstantPoolGen cpg = cg.getConstantPool();
			MethodGen mg = new MethodGen(m, cg.getClassName(), cpg);
			InstructionList il = mg.getInstructionList();
                        isChanged = false;
			for (InstructionHandle handle : il.getInstructionHandles()) {
				if (handle.getInstruction() instanceof LDC) {
					LDC ldc = (LDC) handle.getInstruction();
					if (ldc.getType(cpg).equals(Type.STRING)) {
						int cpIndex = ldc.getIndex();
						String original = ldc.getValue(cpg).toString();
						if (!encryptedStrings.contains(original)) {
                                                        isChanged = true;
							String encrypted = A.decrypt(original);
							ConstantString string = (ConstantString) cpg
									.getConstant(cpIndex);
							int utf8 = string.getStringIndex();
							ConstantUtf8 con = (ConstantUtf8) cpg
									.getConstant(utf8);
                                                        cpg.setConstant(utf8, new ConstantUtf8(encrypted));
							encryptedStrings.add(encrypted);
                                                        UseBCEL.println("The original \"" + original + "\" was encrypted to \"" + encrypted + "\"");
						}
						il.insert(handle.getNext(), invoke);
						callsChanged++;
					}
				}
			}
                        if (isChanged == true) {
                            il.update();
                            mg.setInstructionList(il);
                            mg.setMaxLocals();
                            mg.setMaxStack();
                            mg.removeLocalVariables();
                            mg.update();
                            cg.replaceMethod(m, mg.getMethod());
                        }
		}

	}

}

final class A {
	public final static String decrypt(String encrypted) {
		StringBuilder builder = new StringBuilder();
		byte xor = (byte) (encrypted.length() & 0xFF);
		for (int pos = 0; pos < encrypted.length(); pos++) {
			builder.append((char) (encrypted.charAt(pos) ^ xor));
			xor ^= pos;
		}
		return builder.toString().intern();
	}
}