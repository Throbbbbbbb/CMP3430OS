#include <stdio.h>
#include <unistd.h>
#include "ext2fs.h"

char fileSystem[] = "filesysFile-easy";
int block_size;

#define BLOCK_OFFSET(block) (1024 + (block-1)*block_size)

int main()
{
  int fd, inode_count, group_count, count;

  char boot[1024];
  unsigned char *bitmap;

  struct ext2_super_block super;
  struct ext2_group_desc group_descr;
  struct ext2_inode *inode = malloc(sizeof(struct ext2_inode));

  //Opening the specified fileSystem
  //Make sure you set up the mounted filesystem as well as "flush it" like he talks about in the project
  fd = open(fileSystem, O_RDONLY);

  read(fd, boot, 1024);
  read(fd, &super, sizeof(super));

  //Finding out individial block size
  block_size = 1024 << super.s_log_block_size;

  printf("Block_size: %d\n", block_size);

  //Calculating # of Group Description Blocks we have
  group_count = 1 + (super.s_blocks_count-1) / super.s_blocks_per_group;

  //Plan is to loop through each one and access the inode table for each.
  for(count = 0; count < group_count; count++)
    {
      printf("Group#: %d\n", group_count);
      printf("Inodes Per Group: %d\n", super.s_inodes_per_group);
      
      //Grabbing dat description block
      lseek(fd, 1024 + block_size+(count*sizeof(struct ext2_group_desc)), SEEK_SET);
      read(fd, &group_descr, sizeof(group_descr));

      printf("Total Inodes Count: %d\n", super.s_inodes_count);
 
      //Added some bitmap stuff here that i'm pretty sure you have to use
      bitmap = malloc(block_size);
      lseek(fd, BLOCK_OFFSET(group_descr.bg_block_bitmap), SEEK_SET);
      read(fd, bitmap, block_size);

      //Looping through the inode_table consecutively and searching for an inode with deletion time > 0
      for(inode_count = 1; inode_count <= super.s_inodes_per_group; inode_count++)
	{
	  lseek(fd, BLOCK_OFFSET(group_descr.bg_inode_table)+(inode_count-1)*sizeof(struct ext2_inode), SEEK_SET);
	  read(fd, inode, sizeof(struct ext2_inode));
	  if(inode->i_dtime != 0)
	    {
	      printf("Deleted Inode Found: %d Time: %d\n",inode_count, inode->i_dtime);
	      //This is the point where you need to take the inode and restore it.
	      //This is done using the inode or block bitmap and simply turning it back on
	      //Theres a bunch of other crap to do like reinserting values and then placing and naming the file somewhere
	      //I'll send a bunch of links in a read me that have good info 
	      //As long as it works and we can get an ezpz 60 im all gucci.
	    }
	}
    }

  return 0;
}


