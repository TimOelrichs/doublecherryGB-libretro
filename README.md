## DoubleCherryGB

<img src="/screenshots/logo.png" height="250">



**author:** Tim Oelrichs<br>
**email:** timoelrichs@gmail.com<br>
**website:** [timoelrichs.is-a.dev](https://timoelrichs.is-a.dev)<br>


> DoubleCherryGB is an open source (GPLv2) GB/GBC emulator with up to 16 Player support, based on the TGBDual libretro core by GIGO and Hii.
> It emulates the GAMEBOY 4 PLAYER ADAPTER (DMG-07) and also adds 3-16 Player Link Hacks for two-player games.
> It's named after the Double Cherry Item from Super Mario 3D World.

Check my [website](https://timoelrichs.is-a.dev) for more information, planned features and compatibility list.

<br>

## Features

### Pokemon Stuff
- [Builtin Mew Maschine and weekly GEN1 Pokemon Distributions](#Builtin-Mew-Maschine-and-weekly-GEN1-Pokemon-Distributions)
- [Link Cable Trading over network](#Link-Cable-Trading-over-network), easy Pokemon Gen1 and Gen2 online trading via Lobby (Retroarch 1.17 needed)

  <br>

### Multiplayer
- GAMEBOY 4 PLAYER ADAPTER (DMG-07) emulation (F1-Race, Wave Race, Yoshi's Cookie...) [compatibility list](https://peppermint-theater-276.notion.site/4-player-adapter-compatibility-8d33abe17f1d4bf5a361b368af6622b7)
- up to 16-Player Faceball 2000 emulation (1-15 without bugfix-patch, 16 players needs [patched](https://github.com/Zarithya/Faceball2000DX) rom)
  
- up to 16-Player Tetris Battle Royal Hack (virtual link device, no modified rom needed)
- up to 16-Player Kwirk Multiplayer Hack (virtual link device, no modified rom needed)
- up to 16 Player 4x4Player Adapter
- Of course up to 16 Player Singleplayer for speedrun-battles or if you have arguing kids :D
- local splitscreen
- netplay support

  <br>
  
### Gameboy Addon Emulation
- [TV REMOTE](#TV-REMOTE) Emulation
- [BARCODE BOY](#BARCODE-BOY) Emulation full support (all barcodes are hardcoded, use the numkeys 0-9 for selection)
- [BARCODE TAISEN BARDIGUN](#BARCODE-TAISEN-BARDIGUN) Emulation full Support (all barcodes are hardcoded, use the numkeys 0-8 for selection)
- [POWER ANTENNA/BUGSENSOR](#power-antenna-and-bugsensor) Emulation (LED via led driver overlay & controller rumble on led flash)

### Retroarch-Features
- Retroarchivments support
- Netplay

Netplay is working. Make sure to have all the same amount of emulated gameboys in your core options.


![](/screenshots/F-1%20Race.png)
![](/screenshots/SuperRC.png)
![](/screenshots/TopRankTennis.png)
![](/screenshots/YoshisCookie.png)
![](/screenshots/WaveRace.png)

<br>

## How to

You can download the core via RetroArch Online-Updater on Windows, Linux, Android and IOS.
For Android make sure to use the retroarch APK from retroarch.com and not the GooglePlay Version.

For Multiplayer you have to increase the amount of emulated Gameboy in the core options in your frontend and reload.


<br>

## Builtin Mew Maschine and weekly GEN1 Pokemon Distributions 

DoubleCherryGB has a buildin virtual linkcable device that can trade Pokemon. I called it the PKMBuddy Boy.

- If you don't own a Mew it will trade you a Lv 5 Mew. 
- If you already own a Mew it will give you weekly Pokemon Distribution Events with a choice of different Pokemon. See table below. 
- You can also use this to trade pokemon back for trading evolutions.

You can uses the Numkeys on your keyboard or the SELECT Button to change Pokemon you want to get. 

It's already preconfigured. Leave emulated Gameboys at 1 and just visit the cable club ingame.

Weekly Gen1 Pokemon Distribution List
https://docs.google.com/spreadsheets/d/1SMcpTz_4Mwo1eVAKWB3AbaLHJNthkB5sKHlOPnQw2UI/edit?gid=0#gid=0

<img src="/screenshots/gotMew.png" height="500">

<br>

## Link Cable Trading over network
#### Easy gen1/gen2 pokemon online trading

Why is this easier? No setup and no workarounds. No need to own the same roms (region, edition). Should work cross platform.
You can find trading partners over the netplay lobby (netplay -> refresh Host List), and can use the netplay text-chat by pressing ^ on your keyboard.
Also works in LAN. 

I recommend to use the [RetroArch Discord server](https://discord.com/channels/184109094070779904/326960443157381120) to find players and use voice-chats.

Atleast Retroarch 1.17 is needed for the netpacket api.
Only the linkcable data is transfered over network, so you won't see the others player's screen.

- make sure emulated gameboys is set to 1 in the Core Options 
- load your rom
- start or join a netplay (a second player can join any time)

The netpacket api is activated when you set emulated gameboys to 1. 
This is only meant for trading purposes and may be too slow for actual Multiplayer.
For Multiplayer-sessions please set the amount of emulated gameboys to 2 or higher, because this will activate the old savestate sync for lagfree netplay.  

Trading should work with other GB/GBC Games as well:

- Pokemon R/B/Y/S/G/C
- Pokemon Trading Card Game 1 and 2
- RoboPon Sun/Star
- Medarot 1/2/3 Kabuto/Kuwagata/Parts Collection
- Telefang Speed/Power
- Network Adventure Bugsite Alpha/Beta
- Dragon Warrior (Quest) Monster
- Dragon Warrior (Quest) Monster 2 Tara's Adventure / Cobi's Journey
- Monster Rancher Battle Card Game
- Zelda OoA / OoS ring trade
- ...

Infrared over network is currently not supported (GBE+ does) but maybe in a future release.

Have fun!

<br>

## TV REMOTE

The GBC has a IR receicer and some games used this to unlock cotent with a TV REMOTE.
You can choose between 4 different JVC TV-Remotes. Use SELECT to switch between the Remotes and use the Numpad to press a button.
If you are playing on a handheld you can activate the "autorandom IR Signal" in the Core options.

- Robopon Sun (HUC3) - works
- Bomberman MAX Blue/RED - works
- Bomberman Daman Bakugaiden V - Final Mega Tune - works
- Doraemon Memories - Nobita no Omoide Daibouken - not working

<br>
 
## BARCODE BOY

The BARCODE BOY is a japan exclusive peripheral (See https://shonumi.github.io/articles/art7.html).

The following games have support or even need the BARCODE BOY to be played:

- BATTLE SPACE (needed)
- Monster Maker Barcode Saga (needed)
- Kattobi Road (additional content)
- Family Jockey 2 (additional content)
- Famista 3 (additional content)

All Barcode Cards are hardcoded. You can select different cards with the Numkeys 0-9.
If you don't have a keyboard, the the barcode cards will be changed randomly.

<br>

## BARCODE TAISEN BARDIGUN

A japan exclusive GBC game that came with Barcode Reader(See https://shonumi.github.io/articles/art6.html).

All Barcode Cards are hardcoded. You can select different cards with the Numkeys 0-8 or use the SELECT Button.

<br>

## POWER ANTENNA and BUGSENSOR

These are devices, that came with the games **Keitai Denjuu Telefang** and **Network Adventure Bugsite**  and are plugged into the linkport. It's simply a led that flashes when, something special happens ingame.
DoubleCherryGB uses the controller rumble and the retroarch led api to simulate the led flashing on an overlay.

![](/screenshots/Bugsensor-overlay.gif)

[Here](https://github.com/TimOelrichs/doublecherryGB-libretro/blob/master/overlays/gb-bugsensor.zip) is the example overlay i used over the mega-bezels gbc shader. But you can also make your own.

You need to edit your **retroarch.cfg** file the get the overlay to work. (See https://docs.libretro.com/guides/led-drivers/)

led_driver = "overlay" <br>
led1_map = "1"

<br>

If you like to support me
https://patreon.com/double_cherry_projects<br>
https://www.buymeacoffee.com/timoelrichs<br>

Thanks to
- Shonumi for doing the research and technical documentation for Gameboy and many peripherals/accessories/add-ons  https://shonumi.github.io/index.html
- PSYRaven for the netpacket api and davidgf for his implemtation on gpSP
- Ninoh-Fox for his work on Koriki cfw
- GIGO and Hii for making TGBDual and the libretro port

![](/screenshots/Faceball%202000.png)
![](/screenshots/Tetris.png)
![](/screenshots/tetris_winner.png)
![](/screenshots/Kwirk.png)

Original sources from
- https://github.com/libretro/tgbdual-libretro
- http://gigo.retrogames.com/download.html#tgb-dual
- http://shinh.skr.jp/tgbdualsdl/



