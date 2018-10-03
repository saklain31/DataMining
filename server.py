import threading
from time import sleep
import socket
from random import randint
import sys


#knock = ['who','turnip','cash','Two knee','Pecan']
#knockrep=['You are an owl!','turnip the volume','No, give me peanuts','two knee fish','Pecan someone your own size']

knock = ['who','turnip','cash','Two knee','Pecan','Ketchup','Doughnut','Doris','Nana','Yukon']
knockrep=['You are an owl!','turnip the volume','No, give me peanuts','two knee fish','Pecan someone your own size','Ketchup with me and I’ll tell you!','Doughnut ask, it’s a secret!','Doris locked. Open up!','Nana of your business!','Yukon say that again!']

connection = 0
clientcnt = 0

s = socket.socket()         
print("Socket successfully created")

port = 65000              
s.bind(('', port)) 

print("socket binded to %s" %(port))
s.listen(5) 




def worker(num):
    
    cnt= 0
    tag= 0
    
    global connection
    global clientcnt
    
    tempjoke=-1
    joke = []
    
    
    
    #print("socket is listening")           
    c, addr = s.accept()     
    connection=1
    clientcnt+=1
    print('Got connection from', addr)
    #clicnt=0
    
    while cnt!=10:
        c.send(str.encode('Knock Knock!'))
        print("Knock Knock!")
        
        
        st = c.recv(1024).decode("utf-8")
        print(st)
        
        if(st.lower() != "Who's there?".lower()):
            c.send(str.encode("you are supposed to say, Who's there? Let's try again!"))
            print("you are supposed to say, Who's there? Let's try again!")
            sleep(.5)
            continue
    
            
        if tag==0:
            jokenum = randint(0,9)
        else:
            jokenum = tempjoke
            tag=0
            
            
        while jokenum in joke:
            jokenum = randint(0,9)
            
        print("Joke Index: ",jokenum)
        joke.append(jokenum)
        
        print("server:",knock[jokenum])
        
        c.send(str.encode(knock[jokenum]))
        
        recstr = str(knock[jokenum])+" Who?"
        
        st = c.recv(1024).decode("utf-8")
        
        if(st.lower()==recstr.lower()):
            c.send(str.encode(str(knockrep[jokenum])+"(Want to hear another? Y/N)"))
            cnt+=1
            st = c.recv(1024).decode("utf-8")
            
            print("cnt ",cnt)
            if cnt==10:
                c.send(str.encode("I have no more jokes!"))
            
            if st.lower()=='N'.lower():
                c.close()
                break
            elif st.lower()=='Y'.lower():
                continue
            
        else:
            c.send(str.encode("You are supposed to say, "+ recstr + "Let's start again!"))
            joke.remove(jokenum)
            tempjoke=jokenum
            tag=1
            sleep(.5)
            continue


    c.close()
    clientcnt-=1
    return


threads = []
for i in range(10):
    t = threading.Thread(target=worker, args=(i,))
    threads.append(t)
    t.start()

    
while True:
    sleep(1)
    if connection==0:
        continue
    if clientcnt==0:
        sleep(2)
        if clientcnt==0:
            print("closing",clientcnt)
            sys.exit(0)
            
    