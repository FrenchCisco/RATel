import sqlite3
import os
'''
https://www.sqlitetutorial.net/sqlite-python/creating-database/
When you connect to an SQLite database file that does not exist, SQLite automatically creates the new database for you.
sqlite3
'''

class Sql:
    start =False
    def __init__(self,name_db="RAT-el.sqlite3", name_create_table="table_ratel.sql", name_table="table_ratel"):
        self.name_db = name_db
        self.name_file_table = name_create_table #The ratel table file name
        self.name_table = name_table

        self.conn = self.setConnection()
        self.cursor = self.setCursor()

        Sql.start = True

    def setConnection(self):
    #create a database connection to a SQLite database 
        conn = None
        try:
            conn = sqlite3.connect(self.name_db)
        except sqlite3.Error as e:
            print(e)
        finally:
            return conn

    def setCursor(self):
        if not(self.conn):
            print("[-] The cursor cannot create.")
        else:
            print("[+] Cursor ok.")
            return self.conn.cursor()


    def closeConn(self):
        self.conn.close()


    def readFile(self,name_file):
        #read external file.
        try:
            with open(name_file,"r") as file:
                return str(file.read())
        except IOError as e:
            print(e)
            return ""


    def checkFileExists(self,name):
        true_or_false = None
        try:
            file = open(name)
            true_or_false = True

        except IOError:
            print("File not accessible")
            true_or_false = False
        
        finally:
            file.close()
            return true_or_false
    

    def execSqlCode(self,sql_code, commit=False, display=False):
        try:
            self.cursor.execute(sql_code)
            if(display):
                print(self.cursor.fetchone())

        except sqlite3.Error as e:
            print("Error in execSqlCode: ",e)
        finally:
            if(commit):
                print("commit ok")
                self.conn.commit()
            else:
                pass
    
    def insertInDatabase(self, session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token):
        #print("""INSERT INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}",{},{},"{}","{}","{}","{}")""".format(self.name_table, session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token))
        self.execSqlCode("""INSERT INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}",{},{},"{}","{}","{}","{}")""".format(self.name_table, session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token),True)
        print("[+] Insert in database ok.")       
        

    def updateValue(self,column, value, session,string_value=True):#https://www.tutorialspoint.com/sqlite/sqlite_update_query.htm
        if(string_value):#if value str:
            print("""UPDATE {} SET {} = "{}" WHERE session = {} """.format(self.name_table, column, value, session))
            self.execSqlCode("""UPDATE {} SET {} = "{}" WHERE session = "{}" """.format(self.name_table, column, value, session),True)
        else: #if value int
            print("""UPDATE {} SET {} = {} WHERE session = {} """.format(self.name_table, column, value, session))
            self.execSqlCode("""UPDATE {} SET {} = {} WHERE session = {}""".format(self.name_table, column, value, session),True)
        
        print("[+] UPDATE OK.")


    def removeDatabase(self):
        try:
            os.remove(self.name_db)   
        except Exception as e:
            print(e)
        else:
            print("[+] Database deletes.")
    
    def main(self):
        if(self.checkFileExists(self.name_db)):

            print("[+] The database has been found.")
            self.execSqlCode(self.readFile(self.name_file_table)) #Create table if no exists
            #self.insertInDatabase(4,"127.0.0.1",8888 ,1 ,1 , "C:", "cisco", "01010101")
            #self.insertInDatabase(6,"8.8.8.6666",77978 ,1 ,1 , "C:", "roms", "01010101")

            self.execSqlCode("SELECT * FROM table_ratel",False,True)

            self.updateValue("ip","tamer",1,True)
            self.execSqlCode("SELECT * FROM table_ratel",False,True)

            #self.execSqlCode("""INSERT INTO tables_ratel(session,ip,port,is_he_alive,is_he_admin,path_RAT,username,token) VALUES(1,"8888","127.0.0.1", 1 ,0 , "C:", "cisco", "01010101")""",True)
            #self.removeDatabase()
        else:
            print("[-] The database is probably not found. ")
        #self.removeDatabase()
        self.closeConn()


    
#INSERT INTO tables_ratel(session,ip,port,is_he_alive,is_he_admin,path_RAT,username,token) VALUES(1,"8888","127.0.0.1", 1 ,0 , "C:", "cisco", "01010101");
sqlObject = Sql()
sqlObject.main()


'''
https://stackoverflow.com/questions/15415835/how-to-commit-a-update-in-raw-sql-in-python
'''