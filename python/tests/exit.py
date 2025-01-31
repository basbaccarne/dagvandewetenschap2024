from time import sleep
import sys

print("Woohoow, I'm alive!")
sleep(1)

while(True):
    try:
        print("I'm still alive!")
        sleep(1)

    except KeyboardInterrupt:
        print("You killed me! Thanks for my short life, creator of mine ...")
        sys.exit()
