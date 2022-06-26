# For clarity this Razorcat is the 6 years younger version of HighTerrain66.
# This is an ok text adventure with a substantial amount of cringeworthy content
# On the programming side, it's some of the worst code I've ever written with mountainous if-else blocks, sparse commenting, and unclear identifiers.




#Text Adventure Kittens 2 by Razorcat
#      ###      #       #        #     # #  #   #          ###    ###
#       #  # # ###     # # # # ###     # #     ###     # #   #      #
#       #   #   #      ### # # # #     ##   #   #      # # ###    ###
#       #  # #  ##     # #  #  ###     # #  ##  ##      #  #   ## #
#       #              # #             # #                 ### ## ###
#                              BY RAZORCAT

#                               -LIST OF COMMANDS-
#hit - attacks a nearby object or being(add myself to the end for decreased health)
#north - makes you go north (you can also use n)
#south - makes you go south (you can also use s)
#use - makes you use equipment or technology in the room
#grab - picks up an object
#1, 2, 3, 4 or 5 - makes you go into the numbered doors
#look - makes a description of the room come up
#status - checks your health
#end or suicide - makes you efficiently kill yourself
#typing in the name of an item uses it (e.g "catnip" uses catnip "bfg" uses BFG9000)
#help - brings up help

#                               -NOTES-
#If you find yourself getting a bad ending, try messing around in some rooms. TAKv1 has an easter egg which is shared in this game. There are two bad endings and one good ending.
#You have my, but you cannot enter verb + noun commands such as "open door" or "hit warrior". It's far easier to program, and I can't stand Python list functions. This does make the game simple to do though.
#Please don't claim that this is your game. Feel free to look at the code, but don't copy paste any of it. It won't benefit you whatsoever. I will know if you have copy pasted. You can learn by rewriting this in your own way but copy pasting will only boost your ego, not your programming skills
#This is a very quick game of trial and error- please don't treat it like an AAA title.
#If you find any bugs, please contact me at razorcat64@gmail.com or 14ohill@stanwell.org (preferably the first one)
#This game definietly works on python 3.3 and 3.5. I have not tried it on other versions.
#This is the finsihed version, v2.2. More updates may come in the future.
#Please don't look at the code to see what you have to do. The only gameplay in this is trial and error so there is no point in removing that

import time
import random
health = 100
cmnd = "/: "
room = 0# 0 is entrance, (this was a ditched feature as it's not needed. it might be utilised in a future update)
com = ""
valid = 0
leia = 100
indie = 200
trump = 1
warrior = 20
lc = 0
cmdlist = ["1", "2", "3", "4", "5", "hit", "hit myself", "grab", "shoot", "retreat", "south", "s", "north", "n", "look", "use", "status", "inventory", "suicide", "end", "help", "use", "bfg", "catnip", "americuhn flag"]
inventory = []
def command(inp):
    if inp in cmdlist:
        print("...")
        time.sleep(1)
        return True

def healthcheck():
    if health > 90:
        print("You came out ok")
        time.sleep(1)

    elif health > 70:
        print("You have some cuts and bruises")
        time.sleep(1)

    elif health > 40:
        print("You have an open wound. Consider medical attention.")
        time.sleep(1)

    elif health > 20:
        print("You are bleeding badly and have a broken limb")
        time.sleep(1)

    elif health > 0:
        print("You see your life flash before your eyes...")
        time.sleep(2)
        print("...")
        time.sleep(2)
        print("...")
        time.sleep(2)
    else:
        print("You have died...")
        time.sleep(4)
        exit(0)
        
def entrance():
    global health
    room = 0
    print("You are in a metallic hall with red fluroscent lighting.")
    time.sleep(1)
    print("There are five doors you can go through.")
    time.sleep(1)
    com = input(cmnd)
    while True:
        while not command(com) == True:
            if not com in cmdlist and com != "":
                print("Invalid command")
            com = input(cmnd)
            

        if com == "grab":
            print("There's nothing to grab here")
            com = ""

        elif com == "use":
            print("There's nothing to use here")
            com = ""

        elif com == "hit":
            print("You go to the wall to hit it")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            if random.randint(1, 3) == 2:
                print("Your knuckles bounce straight off the wall and into your face")
                time.sleep(2)
                print("You took 5 damage")
                global health
                health = health - 5
                time.sleep(1)
                healthcheck()
                time.sleep(2)
                print("You notice something strange about the wall...")
                time.sleep(2)
                print("A feeling of horror leaps through your body- It's Donald Trump's wall!")
                time.sleep(3)
                print("You are prodded on the back")
                time.sleep(2)
                print("You turn around and see the man himself!")
                time.sleep(2)
                print("He is holding an M60 machine gun!")
                time.sleep(3)
                print("Trump lets loose, pounding you with high calibre rounds, causing 70 damage.")
                time.sleep(1)
                health = health - 70
                healthcheck()
                time.sleep(2)
                if "bfg" in inventory:
                    print("Unfortunately for him, you have a BFG")
                    time.sleep(2)
                    print("(bfg charge up sound)")
                    time.sleep(2)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You fired the BFG, dealing 140 damage!")
                    time.sleep(2)
                    print("All that is left of the notorious mexican hater is some roasted gibs.")
                    time.sleep(2)
                    inventory.append("americuhn flag")
                    print("You found an americuhn flag!")
                    global trump
                    trump = 0
                else:
                    print("Unfortunately for you, you have bloody bare hands")
                    time.sleep(2)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You hit Trump square in the face, dealing 10 damage!")
                    time.sleep(3)
                    print("Trump lets loose, pounding you with high calibre rounds, causing 70 damage.")
                    time.sleep(1)
                    health = health - 70
                    healthcheck()

            else:
                print("Your knuckles bounce straight off the wall and into your face")
                time.sleep(2)
                print("You took 5 damage")
                health = health - 5
                time.sleep(1)
                healthcheck()

            com = ""

        elif com == "status":
            print("Your health is at " + str(health))
            time.sleep(1)
            com = ""

        elif com == "look":
            print("You are in a metallic hall with red fluroscent lighting to the east and west.")
            time.sleep(1)
            print("There are five doors you can go through to the north.")
            time.sleep(1)
            com = ""

        elif com == "inventory":
            if inventory == []:
                print("You don't have anything but a functioning human body.")
                com = ""

            else:
                print("These are your items")
                print(inventory)
                com = ""
                
            com == ""

        elif com == "end" or com == "suicide":
            print("You find leaves and throw them at yourself.")#this is an easter egg- play zork 3
            time.sleep(2)
            print("A terrific throw! The pile of leaves hits you squarely in the head.")
            time.sleep(3)
            print("Normally, this wouldn't do much damage but by incrdible mischance, you fall over")
            print("backwards trying to duck, and break your neck, justice being swift and merciful in the")
            print("City of Indie.")
            time.sleep(3)
            print("You took 100 damage")
            health -= 100
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "hit myself":
            print("You ready your fists...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You hit yourself square in the face, dealing 10 damage!")
            time.sleep(3)
            print("You took 10 damage")
            health -= 10
            time.sleep(1)
            healthcheck()
            print("Don't self harm kids!")
            com = ""

        elif com == "s" or com == "south":
            print("You go south...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("To your suprise, you walk backwards into a solid metal wall.")
            time.sleep(1)
            print("You took 2 damge")
            health -= 2
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "n" or com == "north":
            print("Which door do you want to go through? (1-5)")

        elif com == "1":
            print("You go through the first door...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            ext()
            com = ""

        elif com == "2":
            print("You go through the second door...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            med()
            com = ""

        elif com == "3":
            print("You go through the third door...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            armoury()
            com = ""

        elif com == "4":
            print("You go through the fourth door...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            security()
            com = ""

        elif com == "5":
            print("You go through the fifth door...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You start falling!")
            time.sleep(1)
            print("...")
            time.sleep(0.5)
            print("...")
            time.sleep(0.5)
            print("...")
            time.sleep(0.5)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(0.1)
            print("...")
            time.sleep(2)
            print("...")
            time.sleep(3)
            print("You land straight into the mouth of a giant kitten.")
            time.sleep(2)
            health -= 200
            print("You took 200 damage")
            time.sleep(2)
            healthcheck()
            com = ""

        elif com == "help":
            print("Here is a list of commands:")
            time.sleep(1)
            print("hit - attacks a nearby object or being(add myself to the end for decreased health)")
            print("north - makes you go north (you can also use n)")
            print("south - makes you go south (you can also use s)")
            print("use - makes you use equipment or technology in the room")
            print("grab - picks up an object")
            print("1, 2, 3, 4 or 5 - makes you go into the numbered doors")
            print("look - makes a description of the room come up")
            print("status - checks your health")
            print("end or suicide - makes you efficiently kill yourself")
            print("help - brings up this again")
            print("typing in the name of an item uses it")
            com = ""

        elif com == "catnip":
            if "catnip" in inventory:
                print("You can't use catnip here.")

            else:
                print("You don't have that item")
            com = ""

        elif com == "bfg":
            if "bfg" in inventory:
                print("Slow down there! You shouldn't use huge laser guns for no reason!")

            else:
                print("You don't have that item")
            com = ""

        elif com == "americuhn flag":
            if "americuhn flag" in inventory:
                print("United States of America The Star Spangled Banner Oh, say! can you see by the dawns early light What so proudly we hailed at the twilights last gleaming; Whose broad stripes and bright stars, through the perilous fight, Oer the ramparts we watched were so gallantly streaming? And the rocket's red glare, the bombs bursting in air, Gave proof through the night that our flag was still there: Oh, say! does that star-spangled banner yet wave Oer the land of the free and the home of the brave? On the shore, dimly seen through the mists of the deep, Where the foes haughty host in dread silence reposes, What is that which the breeze, oer the towering steep, As it fitfully blows, half conceals, half discloses? Now it catches the gleam of the mornings first beam, In fully glory reflected now shines in the stream: Tis the star-spangled banner! Oh, long may it wave Oer the land of the free and the home of the brave! And where is that band who so vauntingly swore That the havoc of war and the battles confusion A home and a country should leave us no more? Their blood has washed out their foul footsteps pollution! No refuge could save the hireling and slave From the terror of flight or the gloom of the grave: And the star-spangled banner in triumph doth wave Oer the land of the free and the home of the brave. Oh, thus be it ever, when freemen shall stand Between their loved home and the wars desolation! Blest with victory and peace, may the heavn-rescued land Praise the Power that hath made and preserved us a nation! Then conquer we must, when our cause it is just, And this be our motto: In God is our trust: And the star-spangled banner in triumph shall wave Oer the land of the free and the home of the brave.")

            else:
                print("You don't have that item")
            com = ""
            
        else:
            com = input(cmnd)

def ext():
    global health
    global indie
    if indie > 0:
        print("A feeling of horror surges through your body as you notice what is in front of you-")
        time.sleep(3)
        print("It's Indie the overlord himself!")
        time.sleep(2)
        print("Use of a huge weapon would be a good idea.")
        time.sleep(2)
        print("There is a large vault door behind Indie- this could be your escape!")

    else:
        print("Your freedom is to the north!")
    time.sleep(2)
    com = input(cmnd)
    while True:
        while not command(com) == True:
            if not com in cmdlist and com != "":
                print("Invalid command")
            com = input(cmnd)
            

        if com == "grab":
            print("There's nothing to grab here")
        
        elif com == "use":
            print("There's nothing to use here")
            com = ""
            
        elif com == "hit":
            if indie > 0:
                print("You go to attack Indie...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You ready your fists...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("Indie bites your arm off!")
                time.sleep(2)
                health -= 50
                healthcheck()
                time.sleep(1)
                print("Indie hits you into the side of the room, dealing 65 damage.")
                health -= 65
                time.sleep(2)
                healthcheck()

            else:
                print("There's nothing to hit here.")

            com = ""

        elif com == "status":
            print("Your health is at " + str(health))
            time.sleep(1)
            com = ""

        elif com == "look":
            if indie > 0:
                print("Indie is to the north.")
                time.sleep(1)
                print("There is a large vault door behind Indie- this could be your escape!")
                time.sleep(2)

            else:
                print("Your freedom is to the north!")
            
            com = ""
            
            
            
            

        elif com == "inventory":
            if inventory == []:
                print("You don't have anything but a functioning human body.")

            else:
                print("These are your items")
                print(inventory)
                com = ""
                
            com == ""

        elif com == "end" or com == "suicide":
            print("You find leaves and throw them at yourself.")#this is an easter egg- play zork 3
            time.sleep(2)
            print("A terrific throw! The pile of leaves hits you squarely in the head.")
            time.sleep(3)
            print("Normally, this wouldn't do much damage but by incrdible mischance, you fall over")
            print("backwards trying to duck, and break your neck, justice being swift and merciful in the")
            print("City of Indie.")
            time.sleep(3)
            print("You took 100 damage")
            health -= 100
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "hit myself":
            print("You ready your fists...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You hit yourself square in the face, dealing 10 damage!")
            time.sleep(3)
            print("You took 10 damage")
            health -= 10
            time.sleep(1)
            healthcheck()
            print("Don't self harm kids!")
            time.sleep(2)
            print("Indie gives you a soft biff, dealing 4 damage")
            health -= 4
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "s" or com == "south":
            print("You go south...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            if indie > 0:
                print("Indie hits you over while you try to escape, dealing 20 damage.")
                health -= 20
                time.sleep(2)
                healthcheck()
                time.sleep(1)
                print("You managed to make it out.")
            entrance()
            com = ""

        elif com == "n" or com == "north":
            if indie > 0:
                print("You go north...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You walk straight towards Indie's paws- not a smart decision.")
                time.sleep(2)
                print("Indie crushes you between his paws, dealing 100 damage.")
                health -= 100
                time.sleep(2)
                healthcheck()
                com = ""

                

            else:
                print("You step towards the light...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(2)
                print("You are in a desert... You decide to carry on going north and hope for safety...")
                time.sleep(3)
                print("You look back at the building behind you...")
                time.sleep(3)
                print("There are huge skyscrapers behind it- the City of Indie.")
                time.sleep(3)
                if "americuhn flag" in inventory:
                    print("You hear the sound of a helicopter nearby...")
                    time.sleep(2)
                    print("You turn around and see one coming in to land!")
                    time.sleep(2)
                    print("This means safety!")
                    time.sleep(3)
                    print("The helicopter touches down and you run towards it")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You hear a thundering noise coming from behind...")
                    time.sleep(3)
                    print("It's Indie!")
                    time.sleep(2)
                    if "catnip" in inventory:
                        print("You throw down the catnip and sprint towards the helicopter")
                        time.sleep(2)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("You turn back and see that Indie is playing with the catnip-")
                        time.sleep(1)
                        print("It worked!")
                        time.sleep(3)
                        print('''???: "Get in now!" ''')
                        time.sleep(2)
                        print("You get into the helicopter")
                        time.sleep(2)
                        print('''???: "Let's get you to saftey."''')
                        time.sleep(3)
                        print("You have managed to escape from Indie the Overlord!")
                        time.sleep(3)
                        print("GOOD END")
                        time.sleep(3)
                        print("...")
                        time.sleep(1)
                        print("             -CREDITS-")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("Scripting by Razorcat")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("Python was originally made by Guido van Rossum")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("This is a python remake/port of a text adventure I made with BASIC called")
                        time.sleep(1)
                        print("The City Of Indie")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("The City Of Indie was meant to be a sequel to TAKv1(Text Adventure Kittens)")
                        time.sleep(1)
                        print("So this is basically a sequel to that game if you haven't already realised.")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        for e in range (1, 200):
                            print("")

                        time.sleep(3)
                        print("Thank you for playing!")
                        time.sleep(3)
                        print("I highly recommend that you try Super Metroid- my favourite game of all time.")
                        time.sleep(2)
                        time.sleep(3)
                        print("TAK3?")
                        time.sleep(3)
                        exit(0)

                    else:
                        print("You sprint towards the helicopter...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("...")
                        time.sleep(0.5)
                        print("Indie is too fast!")
                        time.sleep(3)
                        print("BAD END")
                        time.sleep(3)
                        print("If only you had something to distract Indie...")
                        time.sleep(3)
                        print("...")
                        time.sleep(1)
                        print("             -CREDITS-")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("Scripting by Razorcat")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("Python was originally made by Guido van Rossum")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("This is a python remake/port of a text adventure I made with BASIC called")
                        time.sleep(1)
                        print("The City Of Indie")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("The City Of Indie was meant to be a sequel to TAKv1(Text Adventure Kittens)")
                        time.sleep(1)
                        print("So this is basically a sequel to that game if you haven't already realised.")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        print("")
                        time.sleep(1)
                        time.sleep(3)
                        exit(0)

                else:
                    print("Now you are free from that place you press on...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("             -CREDITS-")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("Scripting by Razorcat")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("Python was originally made by Guido van Rossum")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("This is a python remake/port of a text adventure I made with BASIC called")
                    time.sleep(1)
                    print("The City Of Indie")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("The City Of Indie was meant to be a sequel to TAKv1(Text Adventure Kittens)")
                    time.sleep(1)
                    print("So this is basically a sequel to that game if you haven't already realised.")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    print("")
                    time.sleep(1)
                    for e in range (1, 200):
                        print("")

                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You are feeling very weak...")
                    time.sleep(2)
                    print("There's no way you'll make it anywhere now...")
                    time.sleep(2)
                    print("You collapse onto the hot sands...")
                    time.sleep(2)
                    print("...")
                    time.sleep(2)
                    print("You see a shadow stooping over you...")
                    time.sleep(2)
                    print("It's Indie!")
                    time.sleep(2)
                    print("You accept that you are going to die and that you did everything that you could...")
                    time.sleep(2)
                    print("BAD END...")
                    time.sleep(4)
                    print("Try again for a good ending.")
                    time.sleep(3)
                    exit(0)

        elif com == "help":
            print("Here is a list of commands:")
            time.sleep(1)
            print("hit - attacks a nearby object or being(add myself to the end for decreased health)")
            print("north - makes you go north (you can also use n)")
            print("south - makes you go south (you can also use s)")
            print("use - makes you use equipment or technology in the room")
            print("grab - picks up an object")
            print("1, 2, 3, 4 or 5 - makes you go into the numbered doors")
            print("look - makes a description of the room come up")
            print("status - checks your health")
            print("end or suicide - makes you efficiently kill yourself")
            print("help - brings up this again")
            com = ""

        elif com == "catnip":
            if "catnip" in inventory:
                if indie > 0:
                    print("You hold the catnip out in your hand")
                    time.sleep(2)
                    print("Indie's pupils grow huge and his eyes focus on the catnip!")
                    time.sleep(2)
                    print("Indie leaps at you, dealing 20 damage.")
                    time.sleep(2)
                    healthcheck()
        
                else:
                    print("You may as well hold on to this")

            else:
                print("You don't have that item")
            com = ""

        elif com == "bfg":
            if "bfg" in inventory:
                if indie > 0:
                    print("You bring out the BFG and point it at Indie...")
                    time.sleep(1)
                    print("(bfg charge up sound)")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You fire off an intense orb of energy!")
                    time.sleep(2)
                    print("It hits Indie and deals 140 damage!")
                    indie -= 140
                    time.sleep(2)
                    if indie > 0:
                        print("Indie yowls in pain. Another shot should finish him!")
                        time.sleep(2)
                        print("Indie sends you flying backwards, dealing 80 damage.")
                        time.sleep(2)
                        health -= 80
                        healthcheck()

                    else:
                        print("Indie is thrown backwards by the blast and into the vault door!")
                        time.sleep(1)
                        print("The door is smashed open!")
                        time.sleep(2)
                        print("You can see light outside- freedom!")
                        time.sleep(2)
                        print("Indie is still breathing but he's down for now.")
                        time.sleep(2)
                        if leia > 0:
                            print("The door behind you suddenly opens!")
                            time.sleep(2)
                            print("Leia leaps out of it and attacks you!")
                            time.sleep(1)
                            print("You take 30 damage")
                            time.sleep(2)
                            health -= 30
                            healthcheck()

                        print("Go north to escape!")
                        
            else:
                print("You don't have that item")
            com = ""

        elif com == "americuhn flag":
            if "americuhn flag" in inventory:
                print("United States of America The Star Spangled Banner Oh, say! can you see by the dawns early light What so proudly we hailed at the twilights last gleaming; Whose broad stripes and bright stars, through the perilous fight, Oer the ramparts we watched were so gallantly streaming? And the rocket's red glare, the bombs bursting in air, Gave proof through the night that our flag was still there: Oh, say! does that star-spangled banner yet wave Oer the land of the free and the home of the brave? On the shore, dimly seen through the mists of the deep, Where the foes haughty host in dread silence reposes, What is that which the breeze, oer the towering steep, As it fitfully blows, half conceals, half discloses? Now it catches the gleam of the mornings first beam, In fully glory reflected now shines in the stream: Tis the star-spangled banner! Oh, long may it wave Oer the land of the free and the home of the brave! And where is that band who so vauntingly swore That the havoc of war and the battles confusion A home and a country should leave us no more? Their blood has washed out their foul footsteps pollution! No refuge could save the hireling and slave From the terror of flight or the gloom of the grave: And the star-spangled banner in triumph doth wave Oer the land of the free and the home of the brave. Oh, thus be it ever, when freemen shall stand Between their loved home and the wars desolation! Blest with victory and peace, may the heavn-rescued land Praise the Power that hath made and preserved us a nation! Then conquer we must, when our cause it is just, And this be our motto: In God is our trust: And the star-spangled banner in triumph shall wave Oer the land of the free and the home of the brave.")

            else:
                print("You don't have that item")
            com = ""
        
        else:
            com = input(cmnd)

    

            
def med():
    global health
    global warrior
    global com
    room = 2
    print("You are in a gleaming white room.")
    time.sleep(2)
    if warrior < 1:
        print("There is some medical equipment and catnip.")

    else:
        print("There is a warrior, some medical equipment and catnip")#this is a generic name. i will change it later

    com = input(cmnd)
    while True:
        while not command(com) == True:
            if not com in cmdlist and com != "":
                print("Invalid command")
            com = input(cmnd)
            

        if com == "grab":
            if warrior > 0:
                print("The warrior intervenes!")
                time.sleep(1)
                print("The warrior attacks you, dealing 20 damage.")
                health -= 20
                time.sleep(2)
                healthcheck()

            else:
                print("You take the catnip!")
                time.sleep(2)
                inventory.append("catnip")
            com = ""

        elif com == "use":
            print("You go over to the medical equipment...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            if warrior > 0:
                print("The warrior intervenes!")
                time.sleep(1)
                print("The warrior attacks you, dealing 20 damage.")
                health -= 20
                time.sleep(2)
                healthcheck()

            else:
                if health < 100:
                    print("You use the medical equipment...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    health += 40
                    if health > 100:
                        health = 100
                        
                    print("You regain 40 health!")

                else:
                    print("You do not need to use the medical equipment")
                    time.sleep(1)
            com = ""

        elif com == "hit":
            if warrior > 0:
                print("You go to attack the warrior...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You ready your fists...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You hit the warrior square in the face, dealing 10 damage!")
                time.sleep(3)
                warrior -= 10
                if warrior < 1:
                    print("The warrior drops to the ground with a grunt.")
                    time.sleep(2)

                else:
                    print("The warrior attacks you, dealing 20 damage.")
                    health -= 20
                    time.sleep(2)
                    healthcheck()
            com = ""

            
        elif com == "status":
            print("Your health is at " + str(health))
            time.sleep(1)
            com = ""

        elif com == "look":
            if warrior < 1:
                print("There is some medical equipment and catnip.")

            elif not catnip in inventory:
                print("There is a warrior, some medical equipment and catnip")#this is a generic name. i will change it later

            else:
                print("There is some medical equipment.")
            com = ""

        elif com == "inventory":
            if inventory == []:
                print("You don't have anything but a functioning human body.")
                com = ""

            else:
                print("These are your items")
                print(inventory)
                com = ""
            com == ""

        elif com == "end" or com == "suicide":
            print("You find leaves and throw them at yourself.")#this is an easter egg- play zork 3
            time.sleep(2)
            print("A terrific throw! The pile of leaves hits you squarely in the head.")
            time.sleep(3)
            print("Normally, this wouldn't do much damage but by incrdible mischance, you fall over")
            print("backwards trying to duck, and break your neck, justice being swift and merciful in the")
            print("City of Indie.")
            time.sleep(3)
            print("You took 100 damage")
            health -= 100
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "hit myself":
            print("You ready your fists...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You hit yourself square in the face, dealing 10 damage!")
            time.sleep(3)
            print("You took 10 damage")
            health -= 10
            time.sleep(1)
            healthcheck()
            print("Don't self harm kids!")
            com = ""

        elif com == "s" or com == "south":
            print("You go south...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            entrance()
            com = ""

        elif com == "n" or com == "north":
            if warrior > 0:
                print("You go north...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("While you walk into the wall, the warrior attacks you!")
                time.sleep(1)
                print("You took 25 damage")
                time.sleep(2)
                health -= 25
                healthcheck()
                com = ""

            else:
                print("You go north...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("To your suprise, the floor gives way!")
                time.sleep(1)
                print("...")
                time.sleep(0.5)
                print("...")
                time.sleep(0.5)
                print("...")
                time.sleep(0.5)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(0.1)
                print("...")
                time.sleep(2)
                print("...")
                time.sleep(3)
                print("You unfortunately find yourself in the middle of a gentlekitten's duel.")
                time.sleep(2)
                print("This should be no suprise to you as you are indeed in the splendid City of Indie.")
                time.sleep(2)
                print("The gentlekittens find your sudden appearance rather odd and then ask you if you want to stop for tea and crumpets.")
                time.sleep(3)
                print("After a jolly good show of splendid cuisine, one of the gentlekittens challenges you to a most splendidly sporting duel.")
                time.sleep(2)
                print("Being a proper good sir, you graciously accept the gentlekitten's challenge.")
                time.sleep(3)
                health -= 100
                healthcheck()

            com = ""

        elif com == "help":
            print("Here is a list of commands:")
            time.sleep(1)
            print("hit - attacks a nearby object or being(add myself to the end for decreased health)")
            print("north - makes you go north (you can also use n)")
            print("south - makes you go south (you can also use s)")
            print("use - makes you use equipment or technology in the room")
            print("grab - picks up an object")
            print("1, 2, 3, 4 or 5 - makes you go into the numbered doors")
            print("look - makes a description of the room come up")
            print("status - checks your health")
            print("end or suicide - makes you efficiently kill yourself")
            print("help - brings up this again")
            com = ""

        elif com == "catnip":
            if "catnip" in inventory:
                print("You can't use catnip here.")

            else:
                print("You don't have that item")
            com = ""

        elif com == "bfg":
            if "bfg" in inventory:
                print("Slow down there! You shouldn't use huge laser guns for no reason!")

            else:
                print("You don't have that item")
            com = ""

        elif com == "americuhn flag":
            if "americuhn flag" in inventory:
                print("United States of America The Star Spangled Banner Oh, say! can you see by the dawns early light What so proudly we hailed at the twilights last gleaming; Whose broad stripes and bright stars, through the perilous fight, Oer the ramparts we watched were so gallantly streaming? And the rocket's red glare, the bombs bursting in air, Gave proof through the night that our flag was still there: Oh, say! does that star-spangled banner yet wave Oer the land of the free and the home of the brave? On the shore, dimly seen through the mists of the deep, Where the foes haughty host in dread silence reposes, What is that which the breeze, oer the towering steep, As it fitfully blows, half conceals, half discloses? Now it catches the gleam of the mornings first beam, In fully glory reflected now shines in the stream: Tis the star-spangled banner! Oh, long may it wave Oer the land of the free and the home of the brave! And where is that band who so vauntingly swore That the havoc of war and the battles confusion A home and a country should leave us no more? Their blood has washed out their foul footsteps pollution! No refuge could save the hireling and slave From the terror of flight or the gloom of the grave: And the star-spangled banner in triumph doth wave Oer the land of the free and the home of the brave. Oh, thus be it ever, when freemen shall stand Between their loved home and the wars desolation! Blest with victory and peace, may the heavn-rescued land Praise the Power that hath made and preserved us a nation! Then conquer we must, when our cause it is just, And this be our motto: In God is our trust: And the star-spangled banner in triumph shall wave Oer the land of the free and the home of the brave.")

            else:
                print("You don't have that item")
            com = ""
        
        else:
            com = input(cmnd)

def armoury():
    global leia
    global health
    print("You are in a chrome plated room.")
    time.sleep(1)
    if leia > 0:
        print("There is a fairly large kitten in front of you-")
        time.sleep(2)
        print("It's princess Leia of the city of Indie!")
        time.sleep(2)
        print("She does not exactly look like she would like to share tea and crumpets with you...")
        time.sleep(2)

    if "bfg" in inventory:
        print("There are various weapons on a rack in front of you.")

    elif leia < 1:
        print("There is a weapons rack in front of you-")
        time.sleep(1)
        print("But every gun there is put to shame by this huge laser gun to the side of the rack!")

    else:
        print("There is a weapons rack behind Leia- but all of the weapons are put to shame by a huge gun to the side.")

    time.sleep(1)
    com = input(cmnd)
    while True:
        while not command(com) == True:
            if not com in cmdlist and com != "":
                print("Invalid command")
            com = input(cmnd)
            

        if com == "grab":
            if not "bfg" in inventory:
                if leia < 0:
                    print("With a serious heaving motion, you pick up the bfg!")
                    inventory.append("bfg")

                else:
                    print("You move around Leia...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("With a serious heaving motion, you pick up the bfg!")
                    inventory.append("bfg")

            else:
                print("There's nothing to grab here")
            com = ""

        elif com == "use":
            print("There's nothing to use here")
            com = ""
            
        elif com == "hit":
            if leia > 0:
                print("You go to attack Leia...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You ready your fists...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You hit Leia square in the face, dealing 10 damage!")
                time.sleep(2)
                leia -= 10
                if leia > 0:
                    print("Leia scrunches up her nose.")
                    time.sleep(1)
                    print("Leia bowls you over with her paw then pounces at you, dealing 40 damage")
                    health -= 40
                    time.sleep(2)
                    healthcheck()
                    
                    

                else:
                    print("Leia runs away to the corner of the room and rolls over")
                    time.sleep(2)
                    print("She looks at you with GIGANTIC pupils- adorable!")
                    time.sleep(2)
                    print("You have defeated the Princess of the City of Indie!")
                    time.sleep(2)
                    leia = 0

            else:
                print("Would you really hit such a cute kitten for no reason?")

            com = ""

        elif com == "status":
            print("Your health is at " + str(health))
            time.sleep(1)
            com = ""

        elif com == "look":
            print("You are in a chrome plated room.")
            time.sleep(1)
            if leia > 0:
                print("Leia is to the north.")
                time.sleep(2)

            if "bfg" in inventory:
                print("There are various weapons on a rack to the north.")

            elif leia < 1:
                print("Leia is lying down in the corner and there is a weapons rack to the north-")
                time.sleep(1)
                print("But every gun there is put to shame by this huge laser gun to the side of the rack!")

            else:
                print("There is a weapons rack behind Leia- but all of the weapons are put to shame by a huge gun to the side.")
            
            
            
            

        elif com == "inventory":
            if inventory == []:
                print("You don't have anything but a functioning human body.")

            else:
                print("These are your items")
                print(inventory)
            com == ""

        elif com == "end" or com == "suicide":
            print("You find leaves and throw them at yourself.")#this is an easter egg- play zork 3
            time.sleep(2)
            print("A terrific throw! The pile of leaves hits you squarely in the head.")
            time.sleep(3)
            print("Normally, this wouldn't do much damage but by incrdible mischance, you fall over")
            print("backwards trying to duck, and break your neck, justice being swift and merciful in the")
            print("City of Indie.")
            time.sleep(3)
            print("You took 100 damage")
            health -= 100
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "hit myself":
            print("You ready your fists...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You hit yourself square in the face, dealing 10 damage!")
            time.sleep(3)
            print("You took 10 damage")
            health -= 10
            time.sleep(1)
            healthcheck()
            print("Don't self harm kids!")
            com = ""

        elif com == "s" or com == "south":
            print("You go south...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            entrance()
            com = ""

        elif com == "n" or com == "north":
            if leia > 0:
                print("You go north...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("...")
                time.sleep(1)
                print("You walk straight towards Leia's face- not a smart decision.")
                time.sleep(2)
                print("Leia grabs onto you with her claws and starts chewing on you, dealing 70 damage")
                time.sleep(2)
                healthcheck()
                com = ""

            else:
                print("You can't do that now")

        elif com == "help":
            print("Here is a list of commands:")
            time.sleep(1)
            print("hit - attacks a nearby object or being(add myself to the end for decreased health)")
            print("north - makes you go north (you can also use n)")
            print("south - makes you go south (you can also use s)")
            print("use - makes you use equipment or technology in the room")
            print("grab - picks up an object")
            print("1, 2, 3, 4 or 5 - makes you go into the numbered doors")
            print("look - makes a description of the room come up")
            print("status - checks your health")
            print("end or suicide - makes you efficiently kill yourself")
            print("help - brings up this again")
            com = ""

        elif com == "catnip":
            if "catnip" in inventory:
                if leia > 0:
                    print("You hold the catnip out in your hand")
                    time.sleep(2)
                    print("Leia's pupils grow huge and her eyes focus on the catnip!")
                    time.sleep(2)
                    print("Leia leaps at you, dealing 5 damage.")
                    health -= 5
                    time.sleep(2)
                    healthcheck()
                    print("She takes the catnip from you and starts playing with it in the corner of the room.")
                    time.sleep(2)
                    print("After a while she yawns...")
                    time.sleep(2)
                    print("You have got the Princess of The City of Indie to go to sleep in the the corner of the room!")
                    leia = 0
                    inventory.remove("catnip")

                else:
                    print("You could have given it to Leia earlier...")

            else:
                print("You don't have that item")
            com = ""

        elif com == "bfg":
            if "bfg" in inventory:
                if leia < 1:
                    print("Slow down there! You shouldn't use huge laser guns for no reason!")

                else:
                    print("You point the BFG at Leia...")
                    time.sleep(1)
                    print("(bfg charge up sound)")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("...")
                    time.sleep(1)
                    print("You fire off an intense orb of energy!")
                    time.sleep(2)
                    print("Leia lets out an ear shredding mew!")
                    time.sleep(2)
                    print("The BFG blast turns back at you!")
                    time.sleep(2)
                    if random.randint(1, 3) == 1:
                        print("You take 140 damage by mischance")
                        health -= 140
                        time.sleep(2)
                        healthcheck()

                    else:
                        print("You barely manage to dive out of the way of the blast.")
                        time.sleep(2)
                        print("You take 20 damage")
                        health -= 20
                        time.sleep(2)
                        healthcheck()
            else:
                print("You don't have that item")
            com = ""

        elif com == "americuhn flag":
            if "americuhn flag" in inventory:
                print("United States of America The Star Spangled Banner Oh, say! can you see by the dawns early light What so proudly we hailed at the twilights last gleaming; Whose broad stripes and bright stars, through the perilous fight, Oer the ramparts we watched were so gallantly streaming? And the rocket's red glare, the bombs bursting in air, Gave proof through the night that our flag was still there: Oh, say! does that star-spangled banner yet wave Oer the land of the free and the home of the brave? On the shore, dimly seen through the mists of the deep, Where the foes haughty host in dread silence reposes, What is that which the breeze, oer the towering steep, As it fitfully blows, half conceals, half discloses? Now it catches the gleam of the mornings first beam, In fully glory reflected now shines in the stream: Tis the star-spangled banner! Oh, long may it wave Oer the land of the free and the home of the brave! And where is that band who so vauntingly swore That the havoc of war and the battles confusion A home and a country should leave us no more? Their blood has washed out their foul footsteps pollution! No refuge could save the hireling and slave From the terror of flight or the gloom of the grave: And the star-spangled banner in triumph doth wave Oer the land of the free and the home of the brave. Oh, thus be it ever, when freemen shall stand Between their loved home and the wars desolation! Blest with victory and peace, may the heavn-rescued land Praise the Power that hath made and preserved us a nation! Then conquer we must, when our cause it is just, And this be our motto: In God is our trust: And the star-spangled banner in triumph shall wave Oer the land of the free and the home of the brave.")

            else:
                print("You don't have that item")
            com = ""
        
        else:
            if com == "look":
                com = ""
            com = input(cmnd)

def security():
    global health
    print("You are in a dark room.")
    time.sleep(1)
    print("You can see an array of lights in front of you turn on; monitors...")
    com = input(cmnd)
    while True:
        while not command(com) == True:
            if not com in cmdlist and com != "":
                print("Invalid command")
            com = input(cmnd)
            

        if com == "grab":
            print("There's nothing to grab here")
            com = ""

        elif com == "use":
            print("You go over to the monitors...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            if indie > 0:
                print("In room 1 there is a huge dark brown cat and a vault door behind him.")

            else:
                print("In room 1 there is a hole in the northern wall- your freedom!")

            time.sleep(1)
            if warrior > 0:
                print("In room 2 there is a warrior, medical equipment and catnip.")

            elif not "catnip" in inventory:
                print("In room 2 there is medical equipment and catnip.")
            
            else:
                print("In room 2 there is medical equipment.")

            time.sleep(1)
            if leia > 0:
                print("In room 3 there is a fairly large kitten and a weapons rack.")

            elif not "bfg" in inventory:
                print("In room 3 there is a weapons rack.")

            else:
                print("In room 3 there is a wepons rack.")

            time.sleep(1)
            print("There is no monitor for the fifth room.")
            time.sleep(2)
            com = ""

        elif com == "hit":
            print("Hitting the monitors is probably not a good idea...")
            com = ""

            
        elif com == "status":
            print("Your health is at " + str(health))
            time.sleep(1)
            com = ""

        elif com == "look":
            print("You are in a dark room")
            time.sleep(1)
            print("There is an array of monitors to the north.")
            time.sleep(1)
            com = ""

        elif com == "inventory":
            if inventory == []:
                print("You don't have anything but a functioning human body.")

            else:
                print("These are your items")
                print(inventory)
            com == ""

        elif com == "end" or com == "suicide":
            print("You find leaves and throw them at yourself.")#this is an easter egg- play zork 3
            time.sleep(2)
            print("A terrific throw! The pile of leaves hits you squarely in the head.")
            time.sleep(3)
            print("Normally, this wouldn't do much damage but by incrdible mischance, you fall over")
            print("backwards trying to duck, and break your neck, justice being swift and merciful in the")
            print("City of Indie.")
            time.sleep(3)
            print("You took 100 damage")
            health -= 100
            time.sleep(1)
            healthcheck()
            com = ""

        elif com == "hit myself":
            print("You ready your fists...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("You hit yourself square in the face, dealing 10 damage!")
            time.sleep(3)
            print("You took 10 damage")
            health -= 10
            time.sleep(1)
            healthcheck()
            print("Don't self harm kids!")
            com = ""

        elif com == "s" or com == "south":
            print("You go south...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            print("...")
            time.sleep(1)
            entrance()
            com = ""

        elif com == "n" or com == "north":
            print("There are monitors that way.")
            com = ""

        elif com == "help":
            print("Here is a list of commands:")
            time.sleep(1)
            print("hit - attacks a nearby object or being(add myself to the end for decreased health)")
            print("north - makes you go north (you can also use n)")
            print("south - makes you go south (you can also use s)")
            print("use - makes you use equipment or technology in the room")
            print("grab - picks up an object")
            print("1, 2, 3, 4 or 5 - makes you go into the numbered doors")
            print("look - makes a description of the room come up")
            print("status - checks your health")
            print("end or suicide - makes you efficiently kill yourself")
            print("help - brings up this again")
            com = ""

        elif com == "catnip":
            if "catnip" in inventory:
                print("You can't use catnip here.")

            else:
                print("You don't have that item")
            com = ""

        elif com == "bfg":
            if "bfg" in inventory:
                print("Slow down there! You shouldn't use huge laser guns for no reason!")

            else:
                print("You don't have that item")
            com = ""

        elif com == "americuhn flag":
            if "americuhn flag" in inventory:
                print("United States of America The Star Spangled Banner Oh, say! can you see by the dawns early light What so proudly we hailed at the twilights last gleaming; Whose broad stripes and bright stars, through the perilous fight, Oer the ramparts we watched were so gallantly streaming? And the rocket's red glare, the bombs bursting in air, Gave proof through the night that our flag was still there: Oh, say! does that star-spangled banner yet wave Oer the land of the free and the home of the brave? On the shore, dimly seen through the mists of the deep, Where the foes haughty host in dread silence reposes, What is that which the breeze, oer the towering steep, As it fitfully blows, half conceals, half discloses? Now it catches the gleam of the mornings first beam, In fully glory reflected now shines in the stream: Tis the star-spangled banner! Oh, long may it wave Oer the land of the free and the home of the brave! And where is that band who so vauntingly swore That the havoc of war and the battles confusion A home and a country should leave us no more? Their blood has washed out their foul footsteps pollution! No refuge could save the hireling and slave From the terror of flight or the gloom of the grave: And the star-spangled banner in triumph doth wave Oer the land of the free and the home of the brave. Oh, thus be it ever, when freemen shall stand Between their loved home and the wars desolation! Blest with victory and peace, may the heavn-rescued land Praise the Power that hath made and preserved us a nation! Then conquer we must, when our cause it is just, And this be our motto: In God is our trust: And the star-spangled banner in triumph shall wave Oer the land of the free and the home of the brave.")

            else:
                print("You don't have that item")
            com = ""
        
        else:
            com = input(cmnd)

        
        
                      

    

#init thing(this annotation was added by HighTerrain66, aka Razorcat but 4 years older)
print("###      #       #        #     # #  #   #          ###")
print(" #  # # ###     # # # # ###     # #     ###     # #   #")
print(" #   #   #      ### # # # #     ##   #   #      # # ###")
print(" #  # #  ##     # #  #  ###     # #  ##  ##      #  #  ")
print(" #              # #             # #                 ###")
time.sleep(1)
print("")
time.sleep(1)
print("By Razorcat")
time.sleep(1)
print("")
time.sleep(1)
print("What is your name?")
name = input(cmnd)
print("Welcome to Text Adventure Kittens 2, " + name + "!")
if name == "Swordeye" or name == "Swordeye3000" or name == "Swordeye 3000" or name == "SWORDEYE" or name == "jacob" or name == "Jacob":
    print("Razorcat just so happens to be a big fan of yours- keep uploading!")
    time.sleep(2)
time.sleep(2)
for m in range(1, 200):
    print("")
entrance()
