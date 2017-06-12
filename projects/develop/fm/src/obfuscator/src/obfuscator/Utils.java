package obfuscator;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.util.Map;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.math.BigInteger;
import java.nio.file.Path;
import java.nio.file.Paths;

public class Utils {

    public void DeleteComments(String input, String output) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(input)));
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(output)));
        boolean inBlockComment = false;
        boolean inSlashSlashComment = false;
        int char1 = reader.read();
        if (char1 != -1) {
            int char2;
            while (char1 != -1) {
                if ((char2 = reader.read()) == -1) {
                    writer.write(char1);
                    break;
                }
                if (char1 == '/' && char2 == '*') {
                    inBlockComment = true;
                    char1 = reader.read();
                    continue;
                } else if (char1 == '*' && char2 == '/') {
                    inBlockComment = false;
                    char1 = reader.read();
                    continue;
                } else if (char1 == '/' && char2 == '/' && !inBlockComment) {
                    inSlashSlashComment = true;
                    char1 = reader.read();
                    continue;
                }
                if (inBlockComment) {
                    char1 = char2;
                    continue;
                }
                if (inSlashSlashComment) {
                    if (char2 == '\n') {
                        inSlashSlashComment = false;
                        writer.write(char2);
                        char1 = reader.read();
                        continue;
                    } else if (char1 == '\n') {
                        inSlashSlashComment = false;
                        writer.write(char1);
                        char1 = char2;
                        continue;
                    } else {
                        char1 = reader.read();
                        continue;
                    }
                }
                writer.write(char1);
                char1 = char2;
            }
            writer.flush();
            writer.close();
        }
    }

    public void deleteLineBreaks(String input, String output) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(input)));
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(output)));
        String content = new Scanner(new File(input)).useDelimiter("\\Z").next();
        System.out.println(content);
        content = content.replaceAll("\n", "").replaceAll("\r", "");
        File file = new File(output);
        FileWriter fileWriter = new FileWriter(file);
        fileWriter.write(content);
        fileWriter.flush();
        fileWriter.close();
    }

    // Delete spaces at begin and end of string and replace string in quotes with their hex representation
    public void replaceStrings(String input, String output) {
        try {
            FileInputStream fis = new FileInputStream(input);
            FileOutputStream fos = new FileOutputStream(output);
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
            BufferedReader br = new BufferedReader(new InputStreamReader(fis));
            String line = null;
            try {
                while ((line = br.readLine()) != null) {
                    int start = 0;
                    int end_ = 0;
                    while (true) {
                        start = line.indexOf("\"", start);
                        if (start == -1) break;
                        end_ = line.indexOf("\"", start + 1);
                        if (end_ == -1) {
                            //System.out.println("No end of string, exiting");
                            break;
                        }
                        String replace = "G12365Ghgd4367.fshd1213(\"" + String.format("%x", new BigInteger(1, line.substring(start + 1, end_).getBytes())) + "\")";
                        StringBuilder buf = new StringBuilder(line);
                        buf.replace(start, end_ + 1, replace);
                        line = buf.toString();
                        start = start + 2 + replace.length();
                    }
                    //System.out.println(line);
                    bw.write(line.trim());
                    bw.newLine();
                }
            } catch (Exception ex) {
                Logger.getLogger(Utils.class.getName()).log(Level.SEVERE, "Problem with replacing strings!", ex);
            }
            bw.write("class G12365Ghgd4367 {public static String fshd1213(String a){StringBuilder b = new StringBuilder();StringBuilder t = new StringBuilder();for( int i=0; i<a.length()-1; i+=2 ){String o = a.substring(i, (i + 2));int d = Integer.parseInt(o, 16);b.append((char)d);t.append(d);}return b.toString();}}");
            bw.newLine();
            bw.close();
            br.close();
            fis.close();
            fos.close();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Utils.class.getName()).log(Level.SEVERE, "File not found!", ex);
        } catch (IOException ex) {
            Logger.getLogger(Utils.class.getName()).log(Level.SEVERE, "Error!", ex);
        }
    }

    public String convertStringToHex(String str){
	  char[] chars = str.toCharArray();

	  StringBuffer hex = new StringBuffer();
	  for(int i = 0; i < chars.length; i++){
	    hex.append(Integer.toHexString((int)chars[i]));
	  }
	  return hex.toString();
    }

    public String convertHexToString(String hex){
            StringBuilder sb = new StringBuilder();
            StringBuilder temp = new StringBuilder();
            //49204c6f7665204a617661 split into two characters 49, 20, 4c...
            for( int i=0; i<hex.length()-1; i+=2 ){
                //grab the hex in pairs
                String output = hex.substring(i, (i + 2));
                //convert hex to decimal
                int decimal = Integer.parseInt(output, 16);
                //convert the decimal to character
                sb.append((char)decimal);
                temp.append(decimal);
            }
            //System.out.println("Decimal : " + temp.toString());
            return sb.toString();
    }

    public void changeStandartBufferedReader(String input, String output) throws FileNotFoundException, IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(input)));
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(output)));
        String line;

        while ((line = br.readLine()) != null) {
            if (line.contains("import") && line.contains("BufferedReader")) {
                line = line.replaceAll(line, "import java.io.*;import java.util.Iterator;import java.util.NoSuchElementException;import java.util.Spliterator;import java.util.Spliterators;import java.util.stream.Stream;import java.util.stream.StreamSupport;");
            }
            bw.write(line);
            bw.newLine();
        }
        BufferedReader br2 = new BufferedReader(new InputStreamReader(new FileInputStream("resources\\BufferedReader.java")));
        while ((line = br2.readLine()) != null) {
            bw.write(line);
            bw.newLine();
        }
        bw.close();
        br.close();
    }

    byte[] toByteArray(int value) {
        return ByteBuffer.allocate(4).putInt(value).array();
    }

    int fromByteArray(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getInt();
    }   
    
    void hideInteger (String input, String output, String variable) throws FileNotFoundException, IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(input)));
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(output)));
        String line;

        while ((line = br.readLine()) != null) {
            if (line.contains(variable)) {
                if (line.contains("int")) {
                    line = line.replaceAll("int", "byte[]");
                }
                if (line.contains("=")) {
                    int ind1 = line.indexOf("=");
                    int ind2 = line.indexOf(variable);
                    if (ind1 < ind2) {
                        line = line.replaceAll(variable, "ByteBuffer.wrap("+variable+").getInt()");
                    } else {
                        String substr = line.substring(line.indexOf("=")+1, line.length() - 1).trim();
                        line = line.replaceAll(substr, "ByteBuffer.allocate(4).putInt("+Integer.parseInt(substr)+").array();");
                    }
                }
            }
            bw.write(line);
        }
        br.close();
        bw.close();
    }
}
