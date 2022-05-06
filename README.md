Main branch is the first version written to initial specification
Grid branch has extension tasks implemented

# Infected Bunnies

Write a program that creates a linked list of bunny objects.
Each bunny object must have
Sex: Male, Female (random at creation 50/50)
color: white, brown, black, spotted
age : 0-10 (years old)
Name : randomly chosen at creation from a list of bunny names.
Infected: true/false (decided at time of bunny creation 2% chance of true)

At program initialization 5 bunnies must be created and given random colors.
Each turn afterwards the bunnies age 1 year.
So long as there is at least one male age 2 or older, for each female bunny in the list age 2 or older;
a new bunny is created each turn. (i.e. if there was 1 adult male and 3 adult female bunnies, three new bunnies would be born each turn)
New bunnies born should be the same color as their mother.
If a bunny becomes older than 10 years old, it dies.
If a infected bunny is born then each turn it will infect one other bunny each turn
Infected bunnies are excluded from regular breeding and do not count as adult bunnies.
Infected do not die until they reach age 50.
The program should print a list of all the bunnies in the colony each turn along w/ all the bunnies details, sorted by age.
The program should also output each turns events such as
"Bunny Thumper was born!
Bunny Fufu was born!
Infected Bunny Jason was born!
Bunny Julius Caesar died!
When all the bunnies have died the program terminates.
If the bunny population exceeds 1000 a food shortage must occur killing exactly half of the bunnies (randomly chosen)

★ Modify the program to run in real time, with each turn lasting 2 seconds, and a one second pause between each announcement.

★★ Allow the user to hit the 'k' key to initiate a mass bunny cull! which causes half of all the rabbits to be killed (randomly chosen).

★★★★ Modify the program to place the rabbits in an 80x80 grid. Have the rabbits move one space each turn randomly.
Mark juvenile males with m, adult males w/ M,
juvenile females w/ f, adult females w/ F
infected X

Modify the program so that infected only convert bunnies that end a turn on an adjacent square.
Modify the program so that new babies are born in an empty random adjacent square next to the mother bunny. (if no empty square exits then the baby bunny isn't born)

★★★★★ Modify the program so that it saves each turn to a file and can play back at accelerated speed all subsequent turns.
