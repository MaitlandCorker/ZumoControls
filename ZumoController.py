from tkinter import *
import serial,serial.tools.list_ports
from serial.serialutil import SerialException 
#pip install PySerial
import threading

def send_command(command):
    serialPort.write(command.encode())

def SerialMonitor():
    if serialPort:
        while True:
            try:
                received_message=serialPort.readline()
                print(received_message.decode())
            except SerialException:
                pass
    
serialPort=serial.Serial(port="COM7")

monitorThread=threading.Thread(target=SerialMonitor)
monitorThread.start()


root=Tk()
root.title("Zumo Controller")

label=Label(root,text="i am a label")
label.grid(row=0,column=0)

button=Button(root,text="stop",command=lambda:send_command("stop"))
button.grid(row=1,column=0,padx=10,pady=10,sticky="NESW")

button1=Button(root,text="Turn_Right",command=lambda:send_command("turn_right"))
button1.grid(row=1,column=1,padx=10,pady=10,sticky="NESW")

button2=Button(root,text="Turn_Left",command=lambda:send_command("turn_left"))
button2.grid(row=1,column=2,padx=10,pady=10,sticky="NESW")

button3=Button(root,text="Auto",command=lambda:send_command("auto"))
button3.grid(row=2,column=0,padx=10,pady=10,sticky="NESW")

root.mainloop()
