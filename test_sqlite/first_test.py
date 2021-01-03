import sqlite3

'''
https://www.sqlitetutorial.net/sqlite-python/creating-database/
When you connect to an SQLite database file that does not exist, SQLite automatically creates the new database for you.
'''

class Sql:

    def create_connection(self,db_file):
#create a database connection to a SQLite database 
        conn = None
        try:
            conn = sqlite3.connect(self,db_file)
            print(sqlite3.version)
        except sqlite3.Error as e:
            print(e)
        finally:
            if conn:
                conn.close()
    
    def create_table(self,conn, create_table_sql):
    """ create a table from the create_table_sql statement
    :param conn: Connection object
    :param create_table_sql: a CREATE TABLE statement
    :return:
        """
        try:
            c = conn.cursor()
            c.execute(create_table_sql)
        except sqlite3.Error as e:
            print(e)