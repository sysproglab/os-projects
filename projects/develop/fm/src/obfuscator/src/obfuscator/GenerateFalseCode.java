package obfuscator;

import java.security.SecureRandom;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class GenerateFalseCode {

    public String potentialTypes[] = {"int", "float", "double"};
    public String arith_symbols[] = {"+", "-", "/", "*", "%"};

    public String createWrongCode() {
        FindFunction gencode_ffunc = new FindFunction();
        Random rand = new Random();
        String str;

        str = " {\n";
        for (int i = 0; i < 5; i++) {
            String randVar = gencode_ffunc.generateRandomString(5).toString();
            str += this.potentialTypes[rand.nextInt(2)].toString() + " " + randVar;
            str += "; \n";
            str += randVar + " = " + rand.nextInt() + this.arith_symbols[rand.nextInt(this.arith_symbols.length - 1)];
            str += " " + rand.nextInt() + ";\n";

        }
        str += "}";
        return str;
    }
    
    public String number(){
        Random r = new Random();
        return ""+(r.nextInt(40-1)+1);  
    }
    public String operator(){
        String signs[] = {"+","-","%","*"};
        return ""+(signs[new Random().nextInt(signs.length)]); 
    }
    public String level(int n){
        if(n==1) return number() + 
                        operator() + 
                        number();
        else return level(n-1) + 
                    operator() + 
                    number();
    }
    
    public String createIfStatement() {
        String str = "";
        Random rand = new Random();
        double num = 0;
        String exp = level(rand.nextInt(5)+1);
        str = "int x = "+rand.nextInt(100)+";\n";
        str += "if( "+ implictPredicate(rand.nextInt(100))+" )";
        /*try {
            num = countMathExpression(exp)-rand.nextInt(100)-1;
        } catch (ScriptException ex) {
            System.out.println(ex.getMessage());
        }
        str+=" > "+num+" )";*/
        str+=createWrongCode();
        //System.out.println(str);
        return str;
    }
    public double countMathExpression(String exp) throws ScriptException {
        ScriptEngineManager mgr = new ScriptEngineManager();
        ScriptEngine engine = mgr.getEngineByName("JavaScript");
       
        return  Double.parseDouble(engine.eval(exp).toString());        
    }
    
    public String implictPredicate(int num) {
        String predicates[] = {"~x != x",
            "(x + x & 1) == 0",
            "(x + -x & 1) == 0",
            "(-x & 1) == (x & 1)",
            "((-x ^ x) & 1) == 0",
            "(x * 0x80 & 0x56) == 0",
            "(x << 1 ^ 0x1765) != 0",
            "~(-x * 0x40) != x << 6",
            "(~(x * 0x80) & 0x3d) == 0x3d",
            "x - 0x9d227fa9 != x - 0x699c945e",
            "(x ^ x - 0x35f5f4d2) != 0x42a26409",
            "(x * 0x20000000 & 0x19a27923) == 0",
            "(x * 4 & 8) == (x + x * 3 - 0x1fef9d8f & 8)",
            "(x | 0xffffdbe8) - 0x1baa != x || (x & 0x10) == 0x10",
            "(x ^ 0x1145f) != 0 || (x | 0xfffeffff) == 0xffffffff"
        }; 
        return predicates[num%predicates.length];
    }  
}