package obfuscator.bcel.transform;

import obfuscator.bcel.UseBCEL;
import java.util.Map;
import obfuscator.bcel.NameGenerator;
import org.apache.bcel.classfile.Constant;
import org.apache.bcel.classfile.ConstantClass;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.generic.ArrayType;
import org.apache.bcel.generic.ClassGen;
import org.apache.bcel.generic.ConstantPoolGen;
import org.apache.bcel.generic.Type;

/**
 * Renames classes. 
 */
public class ClassRenamerTransform extends ObTransform {

	// this is static so each instance doesn't just name it's class A
	private static NameGenerator nameGen = new NameGenerator();

	public ClassRenamerTransform(ClassGen cg) {
		super(cg);
		UseBCEL.println("Starting class renamer on class " + cg.getClassName());
	}

	public void execute() {
		if (cg.containsMethod("main", Type.getMethodSignature(Type.VOID, new Type[] { new ArrayType(Type.STRING, 1) })) != null) {
			// we usually want to prevent classes with main methods from being
			// renamed, breaks too many things
			UseBCEL.println(cg.getClassName() + " not renamed as contains as main method");
			return;
		}
		// create the new name for the class, leaves the packages intact.
		String className = cg.getClassName();
		int ind = className.lastIndexOf('.');
		String newName;
		if (ind > -1) {
			newName = className.substring(0, className.lastIndexOf('.')) + '.' + nameGen.next();
		} else {
			newName = nameGen.next();
		}
		cg.setClassName(newName);
		// sets the class name in the actual class file
		String fileName = cg.getFileName();
                UseBCEL.println("fileName: " + fileName);
		int ut = cg.getConstantPool().lookupUtf8(fileName);
		if (ut > -1) {
			// changes the source file attribute
			ConstantUtf8 c = (ConstantUtf8) cg.getConstantPool().getConstant(ut);
			UseBCEL.println("\trenamed source file: " + fileName + " to " + nameGen.current() + ".java");
                        cg.getConstantPool().setConstant(ut, new ConstantUtf8(nameGen.current() + ".java"));
		}
		// fix references to the class
                //printConstantPool(cg, className, newName);
		fixConstantPool(cg, className, newName);
                //printConstantPool(cg, className, newName);
		UseBCEL.println("\t" + className + " renamed to " + newName);
		if (UseBCEL.classes.size() > 0) {
			for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
                                ClassGen c = entry.getValue();
				ConstantPoolGen cpg = c.getConstantPool();
				fixConstantPool(c, className, newName);
				int index = cpg.lookupClass(className);
				if (index > -1) {
					ConstantClass con = (ConstantClass) cpg.getConstant(index);
					int utf = con.getNameIndex();
					ConstantUtf8 utf8 = (ConstantUtf8) cpg.getConstant(utf);
                                        UseBCEL.println("class:" + cpg.getConstant(utf).getClass().getCanonicalName() + ", index: " + index + ", utf: " + utf + ", newName: " + newName.replace(".", "/"));
                                        cg.getConstantPool().setConstant(utf, new ConstantUtf8(newName.replace(".", "/")));
                                        //cg.getConstantPool().setConstant(utf, new ConstantClass(utf)); 
					UseBCEL.println("\t" + className + " renamed to " + newName + " in class " + c.getClassName());
				}
			}
		}
	}

	private void fixConstantPool(ClassGen cg, String className, String newName) {
		ConstantPoolGen cpg = cg.getConstantPool();
		newName = newName.replace('.', '/');
                int counter = 0;
		for (Constant c : cpg.getConstantPool().getConstantPool()) {
			if (c instanceof ConstantUtf8) {
				ConstantUtf8 con = (ConstantUtf8) c;
				String className1 = className.replace('.', '/');
				if (con.getBytes().contains("L" + className1 + ";")) {
					String bytes = con.getBytes().replace('L' + className1 + ';', 'L' + newName + ';');
                                        UseBCEL.println("\treplacing " + con.getBytes() + " to " + bytes);
                                        cpg.setConstant(counter, new ConstantUtf8(bytes));
				}
                                else if (con.getBytes().equals(className1)) {
                                    UseBCEL.println("\treplacing " + con.getBytes() + " to " + newName);
                                    cpg.setConstant(counter, new ConstantUtf8(newName));
                                }
			}
                    counter++;
		}
	}
        
        private void printConstantPool(ClassGen cg, String className, String newName) {
		ConstantPoolGen cpg = cg.getConstantPool();
		newName = newName.replace('.', '/');
		for (Constant c : cpg.getConstantPool().getConstantPool()) {
			if (c instanceof ConstantUtf8) {
				ConstantUtf8 con = (ConstantUtf8) c;
				String className1 = className.replace('.', '/');
				if (con.getBytes().contains("L" + className1 + ";")) {
					UseBCEL.println("\tConstant in pool: " + con.getBytes());
				}
			}
		}
	}
}
