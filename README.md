# thermal-flow-meter-mqqt
Flow heat Meter. I used a nodemcu and a nodemcu development base. ( $5)
2 flow meters connected to pins D7 and D8 ( $16 DN25)
4 ds18b20 connected to pin D4 ($4)
total $25
the hardest thing is setting up and zeroing out the ds18b20 as there is significant difference in temperature reads between different ds18b20s, and finding ds18b20s that give a constant stable temperature read with very little variation. but i allowed an area for ds calibration..
it takes sampling for and publishes at 5 second intervals. in MQTT Json format -
it will publish to topic FlowA and FlowB
it lists:
input temps (c)
output temps (c)
Flow rate ( l/s)
and energy (w) in both negative and positive values (i.e. if you have a heat pump connected to a radiant floor, it will tell the rate at which the floor is aborbing. when the heat pump is on, it will also tell what rate the floor is losing energy at any given moment -- it give's it in near realtime output, and can be graphed like emontx.- which is useful as you can figure out "realtime" COP and solar thermal energy output to the ~ second - as if they were PV panels -- accuracy is reasonable. when error corrected it is about ~0.02- 0.05 degree, which is better than the 0.5c when not error corrected

if using Node-red to process the MQTT packet you will need to pass it through JSON function to convert it from string to an object
in node-red:
MQTT >> JSON>> FREEBOARD 

https://community.openenergymonitor.org/t/esp8266-multiple-flow-heat-meter-for-heat-pumps-and-solar-thermals/3226
