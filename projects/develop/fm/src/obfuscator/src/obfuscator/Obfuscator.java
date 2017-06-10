package obfuscator;

import obfuscator.bcel.UseBCEL;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.List;
import java.util.ArrayList;
import static java.nio.file.StandardCopyOption.*;

public class Obfuscator {

    public static void main (String[] args) throws IOException { 
        List<String> jarPaths = new ArrayList<String>();
        for (int i = 0; i < args.length; i++) {
                String fileName = args[i];
                //System.out.println(args[i]);
                if (fileName.endsWith(".class") || fileName.endsWith(".jar")) {
                    jarPaths.add(args[i]);
                }
                else if (!fileName.endsWith(".java")) {
                    Files.walk(Paths.get(fileName)).forEach(filePath -> {
                        String name = filePath.toString();
                        if (name.endsWith(".class") || name.endsWith(".jar")) {
                            jarPaths.add(name);
                        }
                    });
                }
        }
   
        if ((jarPaths.isEmpty() && args.length < 2)) {
            System.out.println("You must specify input folder with .java files and output file!");
            System.out.println("Or jar file, or class files separated by space!");
            return;
        }

        if (!jarPaths.isEmpty()) {
            UseBCEL bcel = new UseBCEL();
            String []jarPath = new String[jarPaths.size()];
            jarPaths.toArray(jarPath);
            bcel.bcel(jarPath);
        }
        else {
            String folder = args[0];
            String output = args[1];
            String tempFile = "temp.java";
            
            Path fileToDeletePath = Paths.get(output);
            Files.deleteIfExists(fileToDeletePath);
            fileToDeletePath = Paths.get(tempFile);
            Files.deleteIfExists(fileToDeletePath);
            
            FindFunction ff = new FindFunction();
            Utils ut = new Utils();

            WriteAllInOneFile writefile = new WriteAllInOneFile();
            writefile.writeImports(folder, output);
            //writefile.writeMainFile(mainFile, tempFile);
            writefile.findClass(folder, output);

            try {
                ff.writeChangeFunctionsName(output, tempFile);
                ut.DeleteComments(tempFile, output);
                ut.replaceStrings(output, tempFile);
                ut.deleteLineBreaks(tempFile, output);
                
            } catch (IOException ex) {
                Logger.getLogger(Obfuscator.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
