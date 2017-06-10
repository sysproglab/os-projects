package obfuscator.bcel;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Date;
import java.util.Enumeration;
import java.util.List;
import java.util.Locale;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarOutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;
import obfuscator.bcel.transform.ClassRenamerTransform;
import obfuscator.bcel.transform.FieldRenamerTransform;
import obfuscator.bcel.transform.MethodRenamerTransform;
import obfuscator.bcel.transform.ObTransform;
import obfuscator.bcel.transform.UnconditionalBranchTransform;
import obfuscator.bcel.transform.StringEncryptorTransform;
import org.apache.bcel.Repository;
import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.Code;
import org.apache.bcel.classfile.ConstantMethodref;
import org.apache.bcel.classfile.ConstantNameAndType;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.ClassGen;

public class UseBCEL {

    public static Map<String, ClassGen> classes;
    public static List<JarEntry> entries;
    private static final Class<?>[] TRANSFORMS = new Class<?>[]{UnconditionalBranchTransform.class, FieldRenamerTransform.class, MethodRenamerTransform.class, ClassRenamerTransform.class, StringEncryptorTransform.class};
    private static boolean currentlyJar;
    private static PrintWriter logOutput;

    public static boolean isCurrentlyJar() {
        return currentlyJar;
    }

    public static void println(final Object o) {
        System.out.println(o);
    }

    public static void bcel(String[] args) {
        classes = new HashMap<String, ClassGen>();
        if (args.length < 1) {
            System.out.println("Usage: java com.speed.ob.Obfuscate [files...]");
        } else {
            /*try {
                logOutput = new PrintWriter(new File("logs", SimpleDateFormat
                        .getDateTimeInstance(SimpleDateFormat.SHORT, SimpleDateFormat.LONG, Locale.UK)
                        .format(new Date()).replaceAll("[\\/: ]", " ")
                        + ".log"));
            } catch (FileNotFoundException e1) {
                e1.printStackTrace();
            }*/
            UseBCEL.println("Obfuscation is starting!");
            for (int i = 0; i < args.length; i++) {
                String fileName = args[i];
                if (fileName.endsWith(".class") && !fileName.endsWith("_bak.class")) {
                    File file = new File(fileName);
                    if (!file.exists()) {
                        UseBCEL.println(fileName + " doesn't exist!");
                    } else {
                        try {
                            ClassParser cp = new ClassParser(fileName);
                            JavaClass jc = cp.parse();
                            Repository.addClass(jc);
                            ClassGen cg = new ClassGen(jc);
                            classes.put(fileName, cg);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
            
            if (classes.size() > 0) {
                UseBCEL.println("\n\nBefore changing!");
                printCode();
            }
        
            for (int i = 0; i < args.length; i++) {
                String fileName = args[i];
                File file = new File(fileName);
                if (!file.exists()) {
                    UseBCEL.println(fileName + " doesn't exist!");
                } else {
                    UseBCEL.println("\r\nProcessing: " + fileName);
                    if (fileName.endsWith(".class") && !fileName.endsWith("_bak.class")) {
                        currentlyJar = false;
                        ClassGen cg = classes.get(fileName);
                        transformClass(cg);
                    } else if (fileName.endsWith(".jar")) {
                        try {
                            currentlyJar = true;
                            entries = new ArrayList<JarEntry>();
                            transformJar(new JarFile(file));
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    } else {
                        UseBCEL.println(fileName + " not supported.");
                    }
                }
            }
            
            // Write modified classes
            if (currentlyJar == false) {
                for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
                    String fileName = entry.getKey();
                    ClassGen clazz = entry.getValue();
                    new File(fileName).renameTo(new File(fileName.replace(".class", "_bak.class")));
                    try {
                        Path fileToDeletePath = Paths.get(fileName);
                        Files.deleteIfExists(fileToDeletePath);
                        
                        String className = clazz.getClassName();
                        StringBuilder buf = new StringBuilder(className);
                        buf.replace(0, className.lastIndexOf(".") + 1, "");
                        className = buf.toString();
                        
                        buf = new StringBuilder(fileName);
                        buf.replace(fileName.lastIndexOf(File.separator) + 1, fileName.length(), className + ".class");
                        fileName = buf.toString();
                        clazz.getJavaClass().dump(fileName);
                    }catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            
            if (classes.size() > 0) {
                UseBCEL.println("\n\nAfter changing!");
                printCode();
            }
            //logOutput.close();
        }
    }

    private static void transformJar(JarFile jarFile) {
        Enumeration<JarEntry> en = jarFile.entries();
        JarOutputStream out = null;
        try {
            out = new JarOutputStream(new FileOutputStream(jarFile.getName().replace(".jar", "-ob.jar")));
        } catch (IOException e) {
            e.printStackTrace();
        }
        while (en.hasMoreElements()) {
            JarEntry entry = en.nextElement();
            try {
                JarEntry newEntry = new JarEntry(entry.getName());
                if (entry.getName().endsWith(".class")) {
                    ClassParser cp = new ClassParser(jarFile.getInputStream(entry), entry.getName());
                    JavaClass jc = cp.parse();
                    Repository.addClass(jc);
                    ClassGen cg = new ClassGen(jc);
                    entries.add(newEntry);
                    classes.put(entry.getName(), cg);
                } else {
                    out.putNextEntry(newEntry);
                    byte[] buffer = new byte[1024];
                    InputStream in = jarFile.getInputStream(entry);
                    int read;
                    while ((read = in.read(buffer)) != -1) {
                        out.write(buffer, 0, read);
                    }
                    out.flush();
                    out.closeEntry();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for (Class<?> cl : TRANSFORMS) {
            for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
                ClassGen cg = entry.getValue();
                try {
                    ObTransform transform = (ObTransform) cl.getConstructor(ClassGen.class).newInstance(cg);
                    transform.execute();
                } catch (Exception e) {
                    e.printStackTrace();
                    UseBCEL.println(cl.getCanonicalName() + " failed to transform class: " + cg.getClassName());
                }

            }
        }
        for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
            ClassGen cg = entry.getValue();
            // JarEntry newEntry = entries.get(i);
            String name = cg.getClassName().replace('.', '/').concat(".class");
            JarEntry entry2 = new JarEntry(name);
            try {
                out.putNextEntry(entry2);
                out.write(cg.getJavaClass().getBytes());
                out.flush();
                out.closeEntry();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void printCode() {
        for (Map.Entry<String, ClassGen> entry: UseBCEL.classes.entrySet()) {
            ClassGen cg = entry.getValue();
            UseBCEL.println("Class " + cg.getClassName());
            for (Method m : cg.getMethods()) {
                    Code code = m.getCode();
                    UseBCEL.println(m.getName() + ": " + code);
            }
        }
    }
    
    private static void transformClass(ClassGen cg) {
        try {
            for (Class<?> clazz : TRANSFORMS) {
                ObTransform transform = (ObTransform) clazz.getConstructor(ClassGen.class).newInstance(cg);
                transform.execute();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}