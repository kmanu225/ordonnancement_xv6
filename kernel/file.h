#ifndef FILE_H
#define FILE_H

struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE, FD_MUTEX } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;  // FD_INODE and FD_DEVICE
  uint off;          // FD_INODE and FD_DEVICE
  short major;       // FD_DEVICE
  short minor;       // FD_DEVICE
  struct sleeplock mutex; // cette structure n'a de sens que lorsque le champ type sera égal à FD_MUTEX
};

#define major(dev)  ((dev) >> 16 & 0xFFFF)
#define minor(dev)  ((dev) & 0xFFFF)
#define	mkdev(m,n)  ((uint)((m)<<16| (n)))

// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};

// map major device number to device functions.
struct devsw {
  int (*read)(struct file *, int, uint64, int);
  int (*write)(struct file *, int, uint64, int);
};

extern struct devsw devsw[];

#define DISK 0
#define CONSOLE 1
#define WATCHDOG 2

#endif
