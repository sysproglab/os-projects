package test;
import java.util.HashMap;
import java.util.Map;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Path;
class Test {
private static Map<String, User> users = new HashMap<>();

public static void main(String[] args) {
int bbb = 5;

String login = G12365Ghgd4367.fshd1213("6c6f67696e");
User newUser = new User(login, G12365Ghgd4367.fshd1213("70617373776f7264"), G12365Ghgd4367.fshd1213("4d6173686120466f6d696e61"), G12365Ghgd4367.fshd1213("6d617368614079616e6465782e7275"));
users.put(login, newUser);
login = G12365Ghgd4367.fshd1213("6c6f67696e31");
User newUser1 = new User(login, G12365Ghgd4367.fshd1213("70617373776f7264"), G12365Ghgd4367.fshd1213("4d6173686120466f6d696e61"), G12365Ghgd4367.fshd1213("6d61736861314079616e6465782e7275"));
users.put(login, newUser1);
if (login.length() > 0) {
System.out.println(newUser);
System.out.println(G12365Ghgd4367.fshd1213("623a20") + bbb);
System.out.println(G12365Ghgd4367.fshd1213("456d61696c3a20") + newUser1. vNraBGSBuBoSapH ());
System.out.println(G12365Ghgd4367.fshd1213("46756c6c206e616d653a20") + newUser1. poZiQtneqKSXxGK ());
System.out.println(G12365Ghgd4367.fshd1213("4c6f67696e3a20") + newUser1. lVbYHlVixRGNKsi ());
if (!newUser.equals(newUser1)) {
newUser1.getUser();
newUser1. VZMmwRlPZRFDfHi (bbb);
newUser1. XFQdQYfcQtzmHJm (G12365Ghgd4367.fshd1213("70617373776f7264"));
if (newUser1. BcjMUHOzQLuMsoK ()) {
System.out.println(G12365Ghgd4367.fshd1213("49732061757468656e7469636174656421"));
newUser1. jXMLuqGhOqYKKkm ();
}
}
}
int x = 51;
if( (x * 4 & 8) == (x + x * 3 - 0x1fef9d8f & 8) ) {
int  PiARf ;
PiARf  = 1589205834/ -1306202537;
int  IDGOT ;
IDGOT  = -958520989/ -945190591;
float  BPqux ;
BPqux  = -144923887- 1130536346;
float  CpBgy ;
CpBgy  = 1526513003/ -748665053;
int  HkBgu ;
HkBgu  = 1959713489- 78732373;
}

for(Map.Entry<String, User> entry : users.entrySet()) {
String key = entry.getKey();
User value = entry.getValue();
System.out.println(G12365Ghgd4367.fshd1213("4b65793a20") + key);
System.out.println(G12365Ghgd4367.fshd1213("557365723a20") + value);
}

String line;
int len = 0;
try {
System.out.println(G12365Ghgd4367.fshd1213("54727920726561642066696c653a20"));
BufferedReader buffer = new BufferedReader(new FileReader(G12365Ghgd4367.fshd1213("746573742e747874")));
line= buffer.readLine();
while (line!=null){
System.out.println(line);
len = len + line.length();
line= buffer.readLine();
}
System.out.println(G12365Ghgd4367.fshd1213("4c656e6774683a20") + len);
}
catch (FileNotFoundException ie) {
System.out.println(G12365Ghgd4367.fshd1213("46696c65206e6f7420666f756e6421"));
}
catch (IOException ie) {
System.out.println(G12365Ghgd4367.fshd1213("4572726f7221"));
}
}
}
class User {
private int id = 0;
private String login;
private String password;
private String fullName;
private String email;
private boolean authenticated;


public User(String login, String password, String fullName, String email) {
this.login = login;
this.password = password;
this.fullName = fullName;
this.email = email;
authenticated = false;
}


public User(User user) {
id = user.id;
login = user.login;
fullName = user.fullName;
email = user.email;
authenticated = user.authenticated;
}


public String  lVbYHlVixRGNKsi () {
int x = 22;
if( (x + x & 1) == 0 ) {
float  irVnX ;
irVnX  = 1943956742+ -180200547;
float  FqkUd ;
FqkUd  = -1524527684* 1302028529;
int  svLWi ;
svLWi  = -415608635* -1630295506;
float  wmVQA ;
wmVQA  = -510245035/ 1335319421;
float  jvUfe ;
jvUfe  = 915755043+ 477327240;
}
return login;
}


public String  poZiQtneqKSXxGK () {
return fullName;
}


public String  vNraBGSBuBoSapH () {
return email;
}


public void  VZMmwRlPZRFDfHi (int id) {
this.id = id;
}


public boolean  XFQdQYfcQtzmHJm (String password) {
authenticated = this.password.equals(password);
return authenticated;
}


public void  jXMLuqGhOqYKKkm () {
authenticated = false;
}

public String toString() {
return  login + G12365Ghgd4367.fshd1213("3a") + fullName + G12365Ghgd4367.fshd1213("3c") + email + G12365Ghgd4367.fshd1213("3e");
}

public User getUser() {
return this;
}


public boolean  BcjMUHOzQLuMsoK () {
return authenticated;
}
}
class G12365Ghgd4367 {public static String fshd1213(String a){StringBuilder b = new StringBuilder();StringBuilder t = new StringBuilder();for( int i=0; i<a.length()-1; i+=2 ){String o = a.substring(i, (i + 2));int d = Integer.parseInt(o, 16);b.append((char)d);t.append(d);}return b.toString();}}
