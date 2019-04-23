import time 
import RPi.GPIO as GPIO 
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
 
# Setup "callback" funksjon som blir kalt når noe skjer på MQTT nettverket 
# eks: kobler seg til serveren eller mottar en beskjed fra en klient 
def on_connect(client, userdata, flags, rc): 
   print("Connected with result code " + str(rc)) 
   # Abonnerer på on_connect() betyr at man kobler seg til serveren
   # og den kobler til på nytt hvis tilkoblingen blir brutt.
   client.subscribe("/ute/pi")
   # Henter data fra klienter
#Tomme lister som lagrer motatt data.   
Temp = [] 
Humi = []
# "callback" for når en PUBLISH beskjed blir mottatt fra en "publisher" 
def on_message(client, userdata, msg): 
   if msg.topic == '/ute/pi':
       data = float(msg.payload) #motatt data
       if data>500:
           Data = data-500
           Temp.append(Data)
           print(len(Temp))
       else:
           Humi.append(data)
           print(len(Humi))
   if len(Temp)>10 and len(Humi)>10:
       print("I am GROOT")
       #Plotter grafer av motatte verdier
       x_verdier = np.linspace(0, len(Temp)/2, len(Temp))
       plt.figure(1)
       plt.ylabel("C")
       plt.xlabel("tid")
       plt.ylim(0,50)
       plt.title("Temperature")
       plt.plot(x_verdier, Temp)
       plt.savefig("Temp.png")
       plt.figure(2)
       plt.ylabel("%")
       plt.xlabel("tid")
       plt.title("Humidity")
       plt.plot(x_verdier, Humi)
       plt.savefig("Fukt.png")
   timer = time.time()
   if (time.time()-timer)>20:
       print("20 sekunder")
   return Temp,Humi
        
# Lager MQTT klienter og kobler de til den "localhost"
# "localhost": Raspberry PI som kjører denne koden
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
client.connect('localhost', 1883, 60)   
print('Script is running, press Ctrl-C to quit...')
# Setter opp en klient-loop som gaar helt tl den manuelt blir frakoblet
client.loop_forever()