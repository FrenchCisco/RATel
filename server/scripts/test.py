

dict_conn = {0:["0000"], 1:["1111"], 2:["2222"], 3:["2222"],4:["2222"]}

dict_conn[8] = ["8888"]

new_token = "2222"

new_dict = {}

already_dict = False


for key in dict_conn.keys():

    print("First key-->", key)
    print("First Value -->", dict_conn[key])
    if(key == 0):
        new_dict[key] = dict_conn[key]
    
    else:
        
        for sub_key in new_dict.keys():
            if(new_dict[sub_key] == dict_conn[key]):
                already_dict = True
            else:
                pass
        
        if(already_dict):
            print("Value already existing in new dict.")
        else:
            print("Value not exist. ")
            new_dict[key] = dict_conn[key]
        
        
        already_dict = False

    print("\n")
    #teste toutes les liste dans le dictionnaire et determine si la valeur est deja dans le dict_conn. 
    #Si la valeur est presente alors n'ajoute pas dans le nouveau dict, sinon ajoute le.
    

    
    
    '''
    for sub_key in list(new_dict) :
        if(new_dict[sub_key][0] == dict_conn[key][0]):
            print("detect !!!!")
            
        else:
            new_dict[key] = dict_conn[key] 
    '''

print("result: ", new_dict)



