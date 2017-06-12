package obfuscator;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

public class WriteAllInOneFile {

    public String imports[];

    public void findClass(String path, String fileOutput) throws IOException {
        Files.walk(Paths.get(path)).forEach(filePath -> {
            System.out.println(filePath.toString());
            if (!filePath.toString().contains("Obfuscator.java")) {
                FileInputStream fis;

                try {
                    fis = new FileInputStream(filePath.toString());
                    boolean isWrite = false;
                    FileOutputStream fos = new FileOutputStream(fileOutput, true);
                    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
                    BufferedReader br = new BufferedReader(new InputStreamReader(fis));
                    String line = null;
                    int openBrackets = 0, closeBrackets = 0;
                    boolean alreadyPasteFalseCode = false;
                    int inFunc = 0;
                    while ((line = br.readLine()) != null) {
                        if (line.contains("class") && line.contains("{") 
                            && (line.contains("public") || line.contains("private")|| line.contains("protected"))
                            &&!line.contains("(")) {
                                line = line.substring(line.indexOf("class") );
                                isWrite = true;
                                inFunc = 0;
                        }
                        if (inFunc == 0 && (line.contains("public") || line.contains("private")|| line.contains("protected")) 
                                && ((line.contains("void") || line.contains("int")|| line.contains("String")|| line.contains("Double")|| line.contains("Float")))) {
                            alreadyPasteFalseCode = false;
                            inFunc = 1;
                            openBrackets = 0;
                            closeBrackets = 0;
                        }
                        if (inFunc == 1) {
                            openBrackets += line.chars().filter(num -> num == '{').count();
                            closeBrackets += line.chars().filter(num -> num == '}').count();
                        }
                        if (openBrackets == closeBrackets) {
                            inFunc = 0;
                        }
                        if (isWrite) {
                            bw.write(line);
                            bw.newLine();
                            Random rnd = new Random();
                            if (inFunc ==1 && !alreadyPasteFalseCode && !line.contains(" return ") && rnd.nextInt(5)==3) {
                                //System.out.println("Previous line: " + line);
                                GenerateFalseCode fc = new GenerateFalseCode();
                                bw.write(fc.createIfStatement());
                                bw.newLine();
                                alreadyPasteFalseCode = true;
                            }
                        }
                    }
                    bw.close();
                    br.close();
                    fis.close();
                    fos.close();
                } catch (Exception ex) {
                    Logger.getLogger(WriteAllInOneFile.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
    }

    public void writeImports(String path, String fileOutput) throws IOException {
        String[] pkg = new String[1];
        AtomicInteger isUsedBufferedReader = new AtomicInteger();
        isUsedBufferedReader.set(0);
        FileOutputStream fos = new FileOutputStream(fileOutput, true);
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
        
        Files.walk(Paths.get(path)).forEach(filePath -> {
            FileInputStream fis;
            try {
                fis = new FileInputStream(filePath.toString());
                boolean isWrite = false;
                BufferedReader br = new BufferedReader(new InputStreamReader(fis));

                String line = null;
                while ((line = br.readLine()) != null) {
                    if (line.contains("import ") && !line.contains("(")) {
                        bw.write(line);
                        bw.newLine();
                    }
                    if (line.contains("package")) {
                        pkg[0] = line + "\n";
                    }
                }
                
                br.close();
                fis.close();
            } catch (Exception e) {
            }
        });
        
        /*
        bw.write("import java.nio.ByteBuffer;");
        bw.newLine();
        */
        
        String line = null;
        bw.close();
        fos.close();
        
        // Write package at first line
        if (!pkg[0].isEmpty())
        {
            File mFile = new File(fileOutput);
            FileInputStream fis = new FileInputStream(mFile);
            BufferedReader br = new BufferedReader(new InputStreamReader(fis));
            String result = "";
            line = "";
            while( (line = br.readLine()) != null){
                result = result + line + "\n"; 
            }

            result = pkg[0] + result;

            mFile.delete();
            fos = new FileOutputStream(mFile);
            fos.write(result.getBytes());
            fos.flush();
        }
    }

    public void writeMainFile(String fileInput, String fileOutput) throws IOException {
        FileInputStream fis;

        fis = new FileInputStream(fileInput);
        boolean isWrite = false;
        FileOutputStream fos = new FileOutputStream(fileOutput, true);
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
        BufferedReader br = new BufferedReader(new InputStreamReader(fis));
        String line = null;
        while ((line = br.readLine()) != null) {
            if (!line.contains("package"))
            {
                bw.write(line);
                bw.newLine();
            }
        }
        bw.close();
        br.close();
        fis.close();
        fos.close();
    }
}
