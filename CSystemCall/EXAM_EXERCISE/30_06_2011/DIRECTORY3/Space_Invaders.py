#Space Invaders v 1.0

import turtle
import os
import math

#Crea la finestra
window = turtle.Screen()
window.bgcolor("black")
window.title("Space Invaders")

#Crea il bordo
border = turtle.Turtle()
border.speed(0)
border.color("white")
border.penup()
border.setposition(-300, -300)
border.pendown()
border.pensize(3)

#Disegna il bordo x4
for side in range(4):
	border.fd(600)
	border.lt(90)
border.hideturtle()

#Crea player
player = turtle.Turtle()
player.color("blue")
player.shape("triangle")
player.penup()
player.speed(0)
player.setposition(0, -250)
player.setheading(90)

playerspeed = 20

#Crea enemy
enemy = turtle.Turtle()
enemy.color("red")
enemy.shape("circle")
enemy.penup()
enemy.speed(0)
enemy.setposition(-250, 270)

enemyspeed = 20

#Crea proiettile
bullet = turtle.Turtle()
bullet.color("yellow")
bullet.shape("triangle")
bullet.penup()
bullet.speed(0)
bullet.setheading(90)
bullet.shapesize(0.5, 0.5)
bullet.hideturtle()

bulletspeed = 5

#Stato del proiettile
#ready = pronto a sparare
#reloading = in attesa di un altro proiettile

bulletstate = "ready"

#Muovi player
def move_left():
	x = player.xcor()
	x -= playerspeed
	if x < -280:
		x = -280
	player.setx(x)

def move_right():
	x = player.xcor()
	x += playerspeed
	if x > 280:
		x = 280
	player.setx(x)

#Spara il proiettile
def fire_bullet():
	#Permette di gestire lo stato del proiettile in tutto il programma 
	global bulletstate
	
	if bulletstate == "ready":
		bulletstate = "reloading"
		x = player.xcor()
		y = player.ycor() + 10
		bullet.setposition(x, y)
		bullet.showturtle()

#Controlla le collisioni
def isCollision(t1, t2):
	distance = math.sqrt(math.pow(t1.xcor() - t2.xcor(), 2) + math.pow(t1.ycor() - t2.ycor(), 2))
	if distance < 12:
		return True
	else:
		return False

#Collegamenti con tastiera
turtle.listen()
turtle.onkey(move_left, "Left")
turtle.onkey(move_right, "Right")
turtle.onkey(fire_bullet, "space")


#Game Loop
while True:

	#Muovi enemy
	x = enemy.xcor()
	x += enemyspeed
	enemy.setx(x)

	#Inverti la direzione di enemy e scala
	if x == 290:
		enemyspeed *= -1
		y = enemy.ycor()
		y -= 15
		enemy.sety(y)

	if x == -290:
		enemyspeed *= -1
		y = enemy.ycor()
		y -= 15
		enemy.sety(y)


	#Muove il proiettile
	if bulletstate == "reloading":
		y = bullet.ycor()
		y += bulletspeed
		bullet.sety(y)

	#Nascondi il proiettile se supera il margine
	if bullet.ycor() > 285:
		bullet.hideturtle()
		bulletstate = "ready"

	#Nemico colpito
	if isCollision(bullet, enemy):
		bullet.hideturtle()
		bulletstate = "ready"
		bullet.setposition(0, -400)
		enemy.setposition(-250, 270)

	#Perso
	if isCollision(enemy, player):
		print ("Game Over")
		break



delay = raw_input("press enter to finish.")