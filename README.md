Main branch is the first version written to initial specification
Grid branch has extension tasks implemented

# Infected Bunnies

This is the main branch version of the project and has focussed on implementing the main parts of the specification. For my Extension task implementations look at the branch <b>grid</b>.

<pre>
At program initialization, 5 bunnies must be created and given random colours, age can be 0 or random between 0 and 9. 

Each turn, the bunnies age 1 year. 

As long as there is a male (aged 2 or older), for each female in the list (aged 2 or older) a new bunny is created each turn. (E.G. if there was 1 adult male and 3 adult females, three new bunnies would be born each turn) 

New born bunnies should be the same colour as their mother. 

If a bunny becomes older than 10 years old, it dies. 

If an infected bunny is born, each turn it will change one normal bunny into a infected bunny (E.G. If there are two infected bunnies two bunnies will be changed each turn and so on...) 

Infected bunnies are excluded from regular breeding and do not count as adult bunnies. 

Infected do not die until they reach age 50. 

Each turn, the program should print a list of all the bunnies in the colony with all the details, sorted by age. 

The program should also output each turns events such as: 

Bunny Keith was born! 

Bunny Carrol was born! 

Infected Bunny Steve was born! 

Bunny Susan died! 

When all the bunnies have died the program terminates. 

If the bunny population exceeds 1000 a food shortage must occur killing exactly half of the bunnies (randomly chosen) 
</pre>

Additionally in the main branch the following are also implemented.

<pre>
★ Modify the program to run in real time, with each turn lasting 2 seconds, and a one second pause between each announcement. 

★★ Remove the population limit and see how many bunnies your program can handle  

Allow the user to hit the 'k' key to initiate a mass rabbit cull! which causes half of all the rabbits to be killed (randomly chosen). 
</pre>

# Notes before running

* The main file is mainly for providing the different rabbit names and creating the manager instance.

* The programs speed is due to staggering announcements with sleep commands as specified by specification. 

* Across the two different branches each one has a different cull method implementation. The specification stated that the cull should be entirely random however there were concern's about performance and how random it was.

# To do section

* <pre>★★★★ Modify the program so that it saves each turn to a file and can play back at accelerated speed all subsequent turns. </pre> Made practice examples for file IO but had issues merging with project. <br> </br>



