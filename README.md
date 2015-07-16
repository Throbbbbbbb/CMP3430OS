1) http://cs.smith.edu/~nhowe/262/oldlabs/ext2.html

2) http://wiki.osdev.org/Ext2#Determining_the_Number_of_Block_Groups

3) http://www.nongnu.org/ext2-doc/ext2.html

4) http://homepage.smc.edu/morgan_david/cs40/analyze-ext2.htm

https://github.com/ldfaiztt/CSE451/blob/master/projects/project3/undelete.c
- This is the github from project 2 and isn't that related except for the part where they restore the inode. They have some stuff on recreating the file which may be helpful.

I've started writing the report so you'll just have to fill in your half after mine. Also fuck that extra credit shit.

Report
by Gregory Huebner and Brett Recksiedler

The program works by initially loading up the fileSystem specified in the variable fileSystem. It reads it into fd and from there we also obtain our system's super block. The superblock is the most important part of the code as it gives us all the information we need to know.

The first and most important piece of information being the size of each block in the file system. This is done using offsets provided from the super block. We also calculate the total number of group description blocks. Group description blocks make up the system and hold vital information including block/inode bitmaps and the inode table itself, which we can use to find our deleted inodes. 

We enter a for loop to cycle through all of our possible group description blocks. Using offsets we read it in and from there we begin iterating over the inode table. Most of the values for this are pulled from the super block, for example the number of inodes per group. To check for a deleted inode we simply see if its deletion time is not 0. If it not (it should be some huge integer) we can proceed with restoring our once deleted file.
