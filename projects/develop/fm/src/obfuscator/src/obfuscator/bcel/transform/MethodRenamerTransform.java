package obfuscator.bcel.transform;

import obfuscator.bcel.UseBCEL;
import org.apache.bcel.Repository;
import org.apache.bcel.classfile.ConstantMethodref;
import org.apache.bcel.classfile.ConstantNameAndType;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.classfile.LocalVariable;
import org.apache.bcel.generic.ClassGen;

import obfuscator.bcel.resources.GenericSignatureParser;
import java.util.Map;
import java.lang.NullPointerException;
import obfuscator.bcel.NameGenerator;

/**
 * Renames methods in a class and local variables in methods
 */
public class MethodRenamerTransform extends ObTransform {
	private NameGenerator names;

	public MethodRenamerTransform(ClassGen cg) {
		super(cg);
	}

	public void execute() {
		names = new NameGenerator();
		UseBCEL.println("Starting method renamer on " + cg.getClassName());
		// lets rename abstract class methods and interface methods first
		for (Method m : cg.getMethods()) {
                        try {
                            UseBCEL.println(m.getName());
                            for (LocalVariable l: m.getLocalVariableTable().getLocalVariableTable()) {
                                if (!(l.getName().equals("args") && m.getName().equals("main") || l.getName().equals("this"))) {
                                    String newName = names.next();
                                    UseBCEL.println("\t\tLocal variable " + l.getName() + " was renamed to " + newName);
                                    int index = l.getNameIndex();
                                    cg.getConstantPool().setConstant(index, new ConstantUtf8(newName));
                                }
                            }
                        }
                        catch (NullPointerException ie) {}
                        
			if (m.isNative() || m.getName().equals("<clinit>") || m.getName().equals("<init>")
					|| m.getName().equals("main")) {
				// we dont want to mess with native methods, constructors or
				// static initialisers
				continue;
			}
			// look through hierarchy and do not rename any overriden methods
			if (lookThroughHierarchy(cg, m, "")) {
				UseBCEL.println("\tNot renaming: " + m.getName());
				continue;
			}
			// generate a new name
			String oldName = m.getName();
			String newName = names.next();
			UseBCEL.println("\tRenaming " + oldName + " to " + newName);
			// fix the references in this class first
			int utf8 = m.getNameIndex();
			if (utf8 > -1) {
				ConstantUtf8 utf = (ConstantUtf8) cg.getConstantPool().getConstant(utf8);
                              
				// set the new name
				if (utf.getBytes().equals(oldName))
					cg.getConstantPool().setConstant(utf8, new ConstantUtf8(newName));
			}
			fixClass(cg, cg.getClassName(), oldName, newName, m);
			// fix references to this method in the other classes
			for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
                                ClassGen clazz = entry.getValue();
				fixClass(clazz, cg.getClassName(), oldName, newName, m);
			}
		}
	}

	private void fixClass(final ClassGen cg, String clazzName, final String oldName, final String newName,
			final Method m) {
		int index = cg.getConstantPool().lookupMethodref(clazzName, oldName, m.getSignature());
		// look for the method reference
		if (index > -1) {
			ConstantMethodref ref = (ConstantMethodref) cg.getConstantPool().getConstant(index);
			// find the name reference
			int typeInd = ref.getNameAndTypeIndex();
			if (typeInd > -1) {
                                UseBCEL.println("\tUpdating method reference in " + cg.getClassName());
				ConstantNameAndType nameType = (ConstantNameAndType) cg.getConstantPool().getConstant(typeInd);
				int utf8 = nameType.getNameIndex();
				// find the utf8
				if (utf8 > -1) {
					ConstantUtf8 utf = (ConstantUtf8) cg.getConstantPool().getConstant(utf8);
					// set the new name
					if (utf.getBytes().equals(oldName)) {
                                            cg.getConstantPool().setConstant(utf8, new ConstantUtf8(newName));
                                            cg.getConstantPool().setConstant(index, ref.copy());
                                            cg.getConstantPool().addMethodref(clazzName, newName, m.getSignature());
                                        }
                                        UseBCEL.println("Old name: " + utf.getBytes());
                                        index = cg.getConstantPool().lookupMethodref(clazzName, newName, m.getSignature());
                                        if (index > -1) {
                                            ref = (ConstantMethodref) cg.getConstantPool().getConstant(index);
                                            typeInd = ref.getNameAndTypeIndex();
                                            if (typeInd > -1) {
                                                utf8 = nameType.getNameIndex();
                                                // find the utf8
                                                if (utf8 > -1) {
                                                    utf8 = nameType.getNameIndex();
                                                    utf = (ConstantUtf8) cg.getConstantPool().getConstant(utf8);
                                                    UseBCEL.println("New name: " + utf.getBytes());
                                                }
                                            }
                                        }
				}
			}
		}
	}

	private boolean lookThroughHierarchy(ClassGen cg, Method m, String previous) {
		if (cg.isAnnotation())
			return true;
		String superclass = cg.getSuperclassName();
		ClassGen sup = null;
		try {
			JavaClass clazz = Repository.lookupClass(superclass);
			sup = new ClassGen(clazz);
		} catch (ClassNotFoundException e) {
			sup = null;
		}
		if (sup == null) {
			return false;
		}

		// now look through interfaces
		for (String s : cg.getInterfaceNames()) {
			try {
				JavaClass jc = Repository.lookupClass(s);
				for (Method meth : jc.getMethods()) {
					boolean b = false;
					if (meth.getGenericSignature() != null) {
						String s1 = meth.getGenericSignature();
						b = GenericSignatureParser.compareSignatures(m.getSignature(), s1);
					}
					if (meth.getName().equals(m.getName()) && (meth.getSignature().equals(m.getSignature()) || b)) {
						return true;
					}
				}
			} catch (ClassNotFoundException e) {
				UseBCEL.println("\tCannot find interface: " + s);
			}
		}
		for (Method meth : sup.getMethods()) {
			// method is overriden, keep the name
			if (meth.getName().equals(m.getName()) && meth.getSignature().equals(m.getSignature())) {
				return true;
			}

		}
		if (sup.getSuperclassName() == null || previous.equals(sup.getSuperclassName()))
			return false;

		return lookThroughHierarchy(sup, m, superclass);
	}
}
