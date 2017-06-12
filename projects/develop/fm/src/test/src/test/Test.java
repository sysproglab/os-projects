package test;

import java.util.HashMap;
import java.util.Map;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * It is test class
 * @author Masha
 */
public class Test {
    private static Map<String, User> users = new HashMap<>();

    public static void main(String[] args) {
        int bbb = 5;
        // Add users to map
        String login = "login";
        User newUser = new User(login, "password", "Masha Fomina", "masha@yandex.ru");
        users.put(login, newUser);
        login = "login1";
        User newUser1 = new User(login, "password", "Masha Fomina", "masha1@yandex.ru");
        users.put(login, newUser1);
        if (login.length() > 0) {
            System.out.println(newUser);
            System.out.println("b: " + bbb);
            System.out.println("Email: " + newUser1.getEmail());
            System.out.println("Full name: " + newUser1.getFullName());
            System.out.println("Login: " + newUser1.getLogin());
            if (!newUser.equals(newUser1)) {
                newUser1.getUser();
                newUser1.setId(bbb);
                newUser1.signIn("password");
                if (newUser1.isAuthenticated()) {
                    System.out.println("Is authenticated!");
                    newUser1.signOut();
                }
            }
        }
        // Print information about users
        for(Map.Entry<String, User> entry : users.entrySet()) {
            String key = entry.getKey();
            User value = entry.getValue();
            System.out.println("Key: " + key);
            System.out.println("User: " + value);
        }
        // Read file
        String line;
        int len = 0;
        try {
            System.out.println("Try read file: ");
            BufferedReader buffer = new BufferedReader(new FileReader("test.txt"));
            line= buffer.readLine();
            while (line!=null){
                System.out.println(line);
                len = len + line.length();
                line= buffer.readLine();
            }
            System.out.println("Length: " + len);
        }
        catch (FileNotFoundException ie) {
            System.out.println("File not found!");
        }
        catch (IOException ie) {
            System.out.println("Error!");
        }
    }
}
