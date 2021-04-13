import java.sql.*;

public class Main {
    private static final String DEFAULT = "DEFAULT";
    private static final String director = "director";
    private static final String actor = "actor";
    private static final String movie = "movie";
    private static final String award = "award";
    private static final String genre = "genre";
    private static final String movieGenre = "movieGenre";
    private static final String movieObtain = "movieObtain";
    private static final String actorObtain = "actorObtain";
    private static final String directorObtain = "directorObtain";
    private static final String casting = "casting";
    private static final String make = "make";
    private static final String customerRate = "customerRate";
    private static final String customer = "customer";
    private static int movieID, actorID, directorID, awardID,customerID;
    private static Connection connection;
    private static Statement stmt;
    private static ResultSet rs;

    public static void main(String[] args) throws Exception {
        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("Where is your PostgreSQL JDBC Driver? Include in your library path!");
            e.printStackTrace();
            return;
        }
        System.out.println("PostgreSQL JDBC Driver Registered!");
        /// if you have a error in this part, check jdbc driver(.jar file)

        connection = null;

        try {
            connection = DriverManager.getConnection("jdbc:postgresql://127.0.0.1:5432/project_movie", "postgres",
                    "cse3207");
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }
        /// if you have a error in this part, check DB information (db_name, user name,
        /// password)

        if (connection != null) {
            System.out.println(connection);
            System.out.println("You made it, take control your database now!\n\n");
        } else {
            System.out.println("Failed to make connection!\n\n");
        }

        /////////////////////////////////////////////////////
        ////////// write your code on this ////////////
        /////////////////////////////////////////////////////


        assert connection != null;
        stmt = connection.createStatement();

        Query1();
        Query2();
        Query3();

        stmt.close();
        connection.close();
    }

    public static void Query1(){
        /********** create tables **********/
        // create director table
        create(director,
                "directorID SERIAL PRIMARY KEY",
                "directorName varchar(50)",
                "dateOfBirth date",
                "dateOfDeath date");

        // create actor table
        create(actor,
                "actorID SERIAL PRIMARY KEY",
                "actorName varchar(50)",
                "dateOfBirth date",
                "dateOfDeath date",
                "gender varchar(10)");

        // create movie table
        create(movie,
                "movieID SERIAL PRIMARY KEY",
                "movieName varchar(50)",
                "releaseYear int",
                "releaseMonth int",
                "releaseDate int",
                "publisherName varchar(50)",
                "avgRate numeric(5, 2)");

        // create customer table
        create(customer,
                "customerID SERIAL PRIMARY KEY",
                "customerName varchar(50)",
                "dateOfBirth date",
                "gender varchar(10)");

        // create award table
        create(award,"awardID SERIAL PRIMARY KEY", "awardName varchar(50)");

        // create genre table
        create(genre,"genreName varchar(50) PRIMARY KEY");

        // create movieGenre table
        create(movieGenre,
                "movieID int REFERENCES movie",
                "genreName varchar(50) REFERENCES genre",
                "PRIMARY KEY (movieID, genreName)");

        // create movieObtain table
        create(movieObtain,
                "movieID int REFERENCES movie",
                "awardID int REFERENCES award",
                "year int",
                "PRIMARY KEY (movieID, awardID)");

        // create actorObtain table
        create(actorObtain,
                "actorID int REFERENCES actor",
                "awardID int REFERENCES award",
                "year int",
                "PRIMARY KEY (actorID, awardID)");

        // create directorObtain table
        create(directorObtain,
                "directorID int REFERENCES director",
                "awardID int REFERENCES award",
                "year int",
                "PRIMARY KEY (directorID, awardID)");

        // create casting table
        create(casting,
                "movieID int REFERENCES movie",
                "actorID int REFERENCES actor",
                "role varchar(50)",
                "PRIMARY KEY (movieID, actorID)");

        // create make table
        create(make,
                "movieID int REFERENCES movie",
                "directorID int REFERENCES director",
                "PRIMARY KEY (movieID, directorID)");

        // create customerRate table
        create(customerRate,
                "customerID int REFERENCES customer",
                "movieID int REFERENCES movie",
                "rate int",
                "PRIMARY KEY (customerID, movieID)");

        System.out.println("creating tables complete.");




        /********** insert initial data **********/
        ////// insert director data
        insert(false, director, DEFAULT, "'Tim Burton'", "'1958.8.25'");
        insert(false, director, DEFAULT, "'David Fincher'", "'1962.8.28'");
        insert(false, director, DEFAULT, "'Christopher Nolan'", "'1970.7.30'");

        ////// insert actor data
        insert(false, actor, DEFAULT, "'Johnny Depp'", "'1963.6.9'", "null", "'Male'");
        insert(false, actor, DEFAULT, "'Winona Ryder'", "'1971.10.29'", "null", "'Female'");
        insert(false, actor, DEFAULT, "'Mia Wasikowska'", "'1989.10.14'", "null", "'Female'");
        insert(false, actor, DEFAULT, "'Christian Bale'", "'1974.1.30'", "null", "'Male'");
        insert(false, actor, DEFAULT, "'Heath Ledger'", "'1979.4.4'", "'2008.1.22'", "'Male'");
        insert(false, actor, DEFAULT, "'Jesse Eisenberg'", "'1983.10.5'", "null", "'Male'");
        insert(false, actor, DEFAULT, "'Justin Timberlake'", "'1981.1.31'", "null", "'Male'");
        insert(false, actor, DEFAULT, "'Fionn Whitehead'", "'1997.7.18'", "null", "'Male'");
        insert(false, actor, DEFAULT, "'Tom Hardy'", "'1977.9.15'", "null", "'Male'");

        ////// insert movie data
        // movieID(fk), movieName, releaseYear, releaseMonth, releaseDate, publisherName, avgRate
        insert(false, movie, DEFAULT, "'Edward Scissorhands'", "1991", "6", "29", "'20th Century Fox Presents'");
        insert(false, movie, DEFAULT, "'Alice In Wonderland'", "2010", "3", "4", "'Korea Sony Pictures'");
        insert(false, movie, DEFAULT, "'The Social Network'", "2010", "11", "18", "'Korea Sony Pictures'");
        insert(false, movie, DEFAULT, "'The Dark Knight'", "2008", "8", "6", "'Warner Brothers Korea'");
        insert(false, movie, DEFAULT, "'Dunkirk'", "2017", "7", "13", "'Warner Brothers Korea'");

        ////// insert customer data
        insert(false, customer, DEFAULT, "'Ethan'", "'1997.11.14'", "'Male'");
        insert(false, customer, DEFAULT, "'John'", "'1978.1.23'", "'Male'");
        insert(false, customer, DEFAULT, "'Hayden'", "'1980.05.04'", "'Female'");
        insert(false, customer, DEFAULT, "'Jill'", "'1981.4.17'", "'Female'");
        insert(false, customer, DEFAULT, "'Bell'", "'1990.5.14'", "'Female'");

        ////// insert genre data
        insert(false, genre, "'Fantasy'");
        insert(false, genre, "'Romance'");
        insert(false, genre, "'Adventure'");
        insert(false, genre, "'Family'");
        insert(false, genre, "'Drama'");
        insert(false, genre, "'Action'");
        insert(false, genre, "'Mystery'");
        insert(false, genre, "'Thriller'");
        insert(false, genre, "'War'");


        ////// insert movieGenre data
        // movieID, genreName

        // get movieID of Edward
        movieID = findID(movie, "Edward Scissorhands");
        insert(false, movieGenre, Integer.toString(movieID), "'Fantasy'");
        insert(false, movieGenre, Integer.toString(movieID), "'Romance'");

        // get movieID of Alice
        movieID = findID(movie,  "Alice In Wonderland");
        insert(false, movieGenre, Integer.toString(movieID), "'Fantasy'");
        insert(false, movieGenre, Integer.toString(movieID), "'Adventure'");
        insert(false, movieGenre, Integer.toString(movieID), "'Family'");

        // get movieID of Network
        movieID = findID(movie, "The Social Network");
        insert(false, movieGenre, Integer.toString(movieID), "'Drama'");

        // get movieID of Knight
        movieID = findID(movie, "The Dark Knight");
        insert(false, movieGenre, Integer.toString(movieID), "'Action'");
        insert(false, movieGenre, Integer.toString(movieID), "'Drama'");
        insert(false, movieGenre, Integer.toString(movieID), "'Mystery'");
        insert(false, movieGenre, Integer.toString(movieID), "'Thriller'");

        // get movieID of Knight
        movieID = findID(movie, "Dunkirk");
        insert(false, movieGenre, Integer.toString(movieID), "'Action'");
        insert(false, movieGenre, Integer.toString(movieID), "'Drama'");
        insert(false, movieGenre, Integer.toString(movieID), "'Thriller'");
        insert(false, movieGenre, Integer.toString(movieID), "'War'");


        ////// insert casting data
        // movieID, actorID, role

        // Edward Scissorhands
        movieID = findID(movie, "Edward Scissorhands");
        actorID = findID(actor, "Johnny Depp");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");
        actorID = findID(actor, "Winona Ryder");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");

        // Alice In Wonderland
        movieID = findID(movie, "Alice In Wonderland");
        actorID = findID(actor, "Johnny Depp");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");
        actorID = findID(actor, "Mia Wasikowska");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");

        // The Social Network
        movieID = findID(movie, "The Social Network");
        actorID = findID(actor, "Jesse Eisenberg");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");
        actorID = findID(actor, "Justin Timberlake");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Supporting Actor'");

        // The Dark Knight
        movieID = findID(movie, "The Dark Knight");
        actorID = findID(actor, "Christian Bale");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");
        actorID = findID(actor, "Heath Ledger");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");

        // Dunkirk
        movieID = findID(movie, "Dunkirk");
        actorID = findID(actor, "Fionn Whitehead");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Main actor'");
        actorID = findID(actor, "Tom Hardy");
        insert(false, casting, Integer.toString(movieID), Integer.toString(actorID), "'Supporting Actor'");


        ////// insert make data
        // movieID, directorID
        // Edward Scissorhands
        movieID = findID(movie, "Edward Scissorhands");
        directorID = findID(director,"Tim Burton");
        insert(false, make, Integer.toString(movieID), Integer.toString(directorID));

        // Alice In Wonderland
        movieID = findID(movie, "Alice In Wonderland");
        directorID = findID(director,"Tim Burton");
        insert(false, make, Integer.toString(movieID), Integer.toString(directorID));

        // The Social Network
        movieID = findID(movie, "The Social Network");
        directorID = findID(director,"David Fincher");
        insert(false, make, Integer.toString(movieID), Integer.toString(directorID));

        // The Dark Knight
        movieID = findID(movie, "The Dark Knight");
        directorID = findID(director,"Christopher Nolan");
        insert(false, make, Integer.toString(movieID), Integer.toString(directorID));

        // Dunkirk
        movieID = findID(movie, "Dunkirk");
        directorID = findID(director,"Christopher Nolan");
        insert(false, make, Integer.toString(movieID), Integer.toString(directorID));

        System.out.println("Initial data inserted.");
    }

    public static void Query2(){
        /********** insert award data **********/
        // awardID, awardName
        // 2.1
        printStatement("2.1 Winona Ryder won the \"Best supporting actor\" award in 1994");
        insertObtain(actor, "Winona Ryder", "Best supporting actor", 1994);

        // 2.2
        printStatement("2.2 Tom Hardy won the \"Best supporting actor\" award in 2018");
        insertObtain(actor, "Tom Hardy", "Best supporting actor", 2018);

        // 2.3
        printStatement("2.3 Heath Ledger won the \"Best villain actor\" award in 2009");
        insertObtain(actor, "Heath Ledger", "Best villain actor", 2009);

        // 2.4
        printStatement("2.4 Johnny Depp won the \"Best main actor\" award in 2011");
        insertObtain(actor, "Johnny Depp", "Best main actor", 2011);

        // 2.5
        printStatement("2.5 Edward Scissorhands won the \"Best fantasy movie\" award in 1991");
        insertObtain(movie, "Edward Scissorhands", "Best fantasy movie", 1991);

        // 2.6
        printStatement("2.6Alice In Wonderland won the \"Best fantasy movie\" award in 2011");
        insertObtain(movie, "Alice In Wonderland", "Best fantasy movie", 2011);

        // 2.7
        printStatement("2.7 The Dark Knight won the \"Best picture\" award in 2009");
        insertObtain(movie, "The Dark Knight", "Best picture", 2009);

        // 2.8
        printStatement("2.8 Christopher Nolan won the \"Best director\" award in 2018");
        insertObtain(director, "Christopher Nolan", "Best director", 2018);
    }

    public static void Query3() {
        /********** insert rating data **********/
        // customerRate : customerID, movieID, rate
        // update movie rate
        // 3.1
        printStatement("3.1 Ethan rates 5 to “Dunkirk”");
        insertCustomerRate("Ethan", 5, "movieName = 'Dunkirk'", movie);

        // 3.2
        // Edward, Alice
        printStatement("3.2 Bell rates 5 to the movies whose director is “Tim Burton”");
        insertCustomerRate("Bell", 5, "directorName = 'Tim Burton'", make, director);

        // 3.3
        // Edward, Alice
        printStatement("3.3 Jill rates 4 to the movies whose main actor is female");
        insertCustomerRate("Jill", 4, "role = 'Main actor' and gender = 'Female'", casting, actor);

        // 3.4
        // Edward, Alice
        printStatement("3.4 Hayden rates 4 to the fantasy movies");
        insertCustomerRate("Hayden", 4, "genreName = 'Fantasy'", movie, movieGenre);

        // 3.5
        // Knight, Dunkirk
        printStatement("3.5 John rates 5 to the movies whose director won the “Best director” award");
        insertCustomerRate("John", 5, "awardName = 'Best director'", make, directorObtain, award);
    }

    public static void create(String tableName, String...args) {
        try {
            StringBuilder sb = new StringBuilder();
            sb.append("CREATE TABLE if not exists ").append(tableName).append("(");
            int sz = args.length;

            for(int i=0;i<sz;i++) {
                sb.append(args[i]);
                if(i != sz-1) sb.append(", ");
            }

            String sql = sb.append(" );").toString();
            stmt.executeUpdate(sql);

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void insert(boolean isPrinted, String tableName, String... args) {
        try {
            StringBuilder sb = new StringBuilder();
            sb.append("INSERT INTO ").append(tableName).append(" VALUES(");
            int sz = args.length;

            for(int i=0;i<sz;i++) {
                sb.append(args[i]);
                if(i != sz-1) sb.append(", ");
            }

            String sql = sb.append(");").toString();
            stmt.executeUpdate(sql);
            if(isPrinted) printTable(sql, tableName);

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void insertObtain(String objectName, String conditionValue, String awardName, int year) {
        int objectID = findID(objectName, conditionValue);
        awardID = findID(award, awardName);

        // award가 등록이 되어 있지 않을 경우
        if(awardID == -1) {
            // award 등록(insert)
            insert(true, award, DEFAULT, "'"+awardName+"'");
            awardID = findID(award, awardName);
        }

        // insert obtain data
        insert(true, objectName+"Obtain", Integer.toString(objectID), Integer.toString(awardID), Integer.toString(year));
    }

    public static void updateMovieRate(int movieID, double newRate) {
        String sql = "UPDATE movie SET avgRate = " + newRate + " WHERE movieID = " + movieID;
        try{
            stmt.executeUpdate(sql);
            printTable(sql, "movie");
        } catch (SQLException e){
            e.printStackTrace();
        }
    }

    public static void insertCustomerRate(String customerName, int ratingScore, String condition, String... joinTables) {
        //// 1. customerRate에 데이터 추가
        // customer에서 customerID 가져오기
        customerID = findID(customer,  customerName);
        // movie에서 movieID 가져오기
        String sql = findMovieIDByJoin(condition, joinTables);
        System.out.println(sql);
        try {
            Statement selectStmt = connection.createStatement();
            ResultSet selectRs = selectStmt.executeQuery(sql);

            while (selectRs.next()) {
                movieID = selectRs.getInt(1);
                // customerRate에 저장
                insert(true, customerRate, Integer.toString(customerID), Integer.toString(movieID), Integer.toString(ratingScore));
            }
            selectStmt.close();

        } catch (SQLException e){
            e.printStackTrace();
        }

        //// 2. movie 평점 업데이트
        // customerRate에서 movieID 별로 rate 평균내기
        try {
            Statement rateStmt = connection.createStatement();
            sql = "SELECT movieID, newRate " +
                    "FROM (" +
                    "SELECT round(avg(rate), 2) AS newRate , movieID " +
                    "FROM customerRate " +
                    "GROUP BY movieID " +
                    ") avg_rate " +
                    "NATURAL JOIN movie " +
                    "WHERE avgrate is null OR avgrate != newRate;";
            ResultSet rateRs = rateStmt.executeQuery(sql);

            while (rateRs.next()) {
                movieID = rateRs.getInt(1);
                double newRate = rateRs.getDouble(2);
                // movie에 aveRate 업데이트
                updateMovieRate(movieID, newRate);
            }
            rateStmt.close();
        } catch (SQLException e){
            e.printStackTrace();
        }
    }

    public static int findID(String tableName, String conditionValue){
        String ID = tableName + "ID";
        String conditionColumn = tableName + "Name";

        try {
            String sql = "SELECT " + ID +
                    " FROM " + tableName +
                    " WHERE " + conditionColumn + " = '" + conditionValue + "'";
            rs = stmt.executeQuery(sql);
            if(rs.next()) return rs.getInt(ID);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return -1;
    }

    public static String findMovieIDByJoin(String condition, String... joinTables){
        StringBuilder sb = new StringBuilder();
        sb.append("SELECT movieID FROM ");
        int sz = joinTables.length;

        for(int i=0;i<sz;i++) {
            sb.append(joinTables[i]);
            if(i != sz-1) sb.append(" natural join ");
        }

        sb.append(" WHERE ").append(condition);
        return sb.toString();
    }

    public static void printTable(String translatedSQL, String tableName){
        System.out.println("Translated SQL : " + translatedSQL);
        System.out.printf("-----< %s >-----\n", tableName);

        try {
            String sql = "SELECT * FROM " + tableName;
            rs = stmt.executeQuery(sql);
            ResultSetMetaData rsmd = rs.getMetaData();
            int columnCnt = rsmd.getColumnCount();

            // print culumn
            for(int i=1;i<=columnCnt;i++){
                System.out.printf("|%-25s", rsmd.getColumnName(i));
            }
            System.out.print("\n");

            // print rows
            while(rs.next()){
                for(int i=1;i<=columnCnt;i++){
                    System.out.printf("|%-25s", rs.getObject(i));
                }
                System.out.print("\n");
            }

        } catch (SQLException e){
            e.printStackTrace();
        }
    }

    public static void printSelectResult(ResultSet rs){

    }

    public static void printStatement(String statement){
        System.out.printf("\n[::Statement::] %s\n", statement);
    }
}
