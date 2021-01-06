import sqlite3
import os
'''
https://www.sqlitetutorial.net/sqlite-python/creating-database/
When you connect to an SQLite database file that does not exist, SQLite automatically creates the new database for you.
sqlite3
'''

class Sql:
    
    def __init__(self,name_db, name_file_table, name_table):
        self.name_db = name_db
        self.name_file_table = name_file_table #The ratel table file name
        self.name_table = name_table
        self.conn = self.setConnection()
        self.cursor = self.setCursor()
        print("CONN: ", self.conn)
        print("CURSOR: ",self.cursor)
        print("CONSTRUCTOR CALLED")
        
    def setConnection(self):
    #create a database connection to a SQLite database 
        conn = None
        try:
            conn = sqlite3.connect(self.name_db, check_same_thread=False) #https://stackoverflow.com/questions/48218065/programmingerror-sqlite-objects-created-in-a-thread-can-only-be-used-in-that-sa/60902437
        except sqlite3.Error as e:
            print(e)
        finally:
            return conn

    def setCursor(self):
        if not(self.conn):
            print("[-] The cursor cannot create.")
        else:
            #print("[+] Cursor ok.")
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
            print(sql_code)
            self.cursor.execute(sql_code)
            if(display):
                print(self.cursor.fetchone())

        except sqlite3.Error as e:
            print("Error in execSqlCode: ",e)
        finally:
            if(commit):
                #print("commit ok")
                self.conn.commit()
            else:
                pass

    def selectAll(self):
        list_of_rows = []
        '''
        returns a list of tuple,
        each row in the database and store in a tuple.
        '''        
        try:
            self.cursor.execute("""SELECT * FROM {}""".format(self.name_table))
            rows = self.cursor.fetchall()
            
            for row in rows: #row = tuple
                list_of_rows.append(row)

        except sqlite3.Error as e:
            print("Error in execSqlCode: ",e)
        finally:        
            #print("commit ok")
            self.conn.commit()
            
            return list_of_rows


    def insertInDatabase(self, session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token):
        #print("""INSERT INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}","{}","{}","{}","{}","{}","{}") """.format(self.name_table, session ,ip, port, is_he_alive, is_he_admin, path_RAT, username, token))
        print("""INSERT INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}",{},"{}","{}","{}","{}","{}")""".format(self.name_table, session ,ip, port, is_he_alive, is_he_admin, path_RAT, username, token))
        self.execSqlCode("""INSERT INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}",{},"{}","{}","{}","{}","{}")""".format(self.name_table, session ,ip, port, is_he_alive, is_he_admin, path_RAT, username, token),True)
        print("[+] Insert in database ok.")       
        

    def updateValue(self, column, value, session,is_string=False):#https://www.tutorialspoint.com/sqlite/sqlite_update_query.htm
        if not(is_string):
            print("""UPDATE {} SET {} = {} WHERE session = {} """.format(self.name_table, column, value, session))
            self.execSqlCode("""UPDATE {} SET {} = {} WHERE session = {} """.format(self.name_table, column, value, session),True)
        else:
            print("""UPDATE {} SET {} = "{}" WHERE session = {} """.format(self.name_table, column, value, session))
            self.execSqlCode("""UPDATE {} SET {} = "{}" WHERE session = {} """.format(self.name_table, column, value, session),True)
        
        print("[+] UPDATE OK.")

    
    def returnValue(self, session, column): 
        row = ""
        try:
            print("""SELECT {} FROM {} WHERE session = {} """.format(column, self.name_table ,session))
            self.cursor.execute("""SELECT {} FROM {} WHERE session = {} """.format(column, self.name_table ,int(session) ))
            row = self.cursor.fetchone()  #return tuple
            print(row)
        except sqlite3.Error as e:
            print("Error in execSqlCode: ",e)
        finally:
            self.conn.commit()
            return str(row[0])
    
    def returnLastSession(self):
        try:
            self.cursor.execute("""SELECT MAX(session) FROM {}""".format(self.name_table))
            row = self.cursor.fetchone()  #return tuple
            
        #    print("-->",type(row),len(row))
        
        except sqlite3.Error as e:
            print("Error in execSqlCode: ",e)
        finally:
            self.conn.commit()
            return int(row[0])

    def createDb(self):
        self.execSqlCode(self.readFile(self.name_file_table)) #Create table if no exists

    def removeDatabase(self):
        
        try:
            os.remove(self.name_db)   
        except Exception as e:
            print(e)
        
    def setTrueOrFalse(self,data):
        #Convert a string from the database to Boolean
        if("False" in data):
            return False
        elif("True" in data):
            return True
        else:
            return data

    def main(self):
        if(self.checkFileExists(self.name_db)):

            print("[+] The database has been found.")
            #self.insertInDatabase(4,"127.0.0.1",8888 ,1 ,1 , "C:", "cisco", "01010101")
            #self.insertInDatabase(6,"8.8.8.6666",77978 ,1 ,1 , "C:", "roms", "01010101")

            self.execSqlCode("SELECT * FROM table_ratel",False,True)

            #self.updateValue("ip","tamer",1,True)
            self.execSqlCode("SELECT * FROM table_ratel",False,True)

            #self.execSqlCode("""INSERT INTO tables_ratel(session,ip,port,is_he_alive,is_he_admin,path_RAT,username,token) VALUES(1,"8888","127.0.0.1", 1 ,0 , "C:", "cisco", "01010101");
            #self.removeDatabase()
        else:
            print("[-] The database is probably not found. ")
        #self.removeDatabase()
        self.closeConn()


    
#INSERT INTO tables_ratel(session,ip,port,is_he_alive,is_he_admin,path_RAT,username,token) VALUES(1,"8888","127.0.0.1", 1 ,0 , "C:", "cisco", "01010101");

'''
https://stackoverflow.com/questions/15415835/how-to-commit-a-update-in-raw-sql-in-python
'''