package test;

import java.nio.file.Path;

/* Class for description user */
public class User {
    private int id = 0;
    private String login;
    private String password;
    private String fullName;
    private String email;
    private boolean authenticated;

    // Constructor
    public User(String login, String password, String fullName, String email) {
        this.login = login;
        this.password = password;
        this.fullName = fullName;
        this.email = email;
        authenticated = false;
    }

    // Other constructor
    public User(User user) {
        id = user.id;
        login = user.login;
        fullName = user.fullName;
        email = user.email;
        authenticated = user.authenticated;
    }

    /*
        Method for getting login
    */
    public String getLogin() {
        return login;
    }

    /*
        Method for getting full name
    */
    public String getFullName() {
        return fullName;
    }

    /*
        Method for getting email
    */
    public String getEmail() {
        return email;
    }

    // Method for setting id
    public void setId(int id) {
        this.id = id;
    }
    
    // Method for sign in of user
    public boolean signIn(String password) {
        authenticated = this.password.equals(password);
        return authenticated;
    }

    // Method for sign out
    public void signOut() {
        authenticated = false;
    }

    public String toString() {
        return  login + ":" + fullName + "<" + email + ">";
    }

    public User getUser() {
        return this;
    }

    /**
     * Method to get authentication state of user
     * @return true if user is authenticated
     */
    public boolean isAuthenticated() {
        return authenticated;
    }
}