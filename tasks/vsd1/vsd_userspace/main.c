#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "vsd_ioctl.h"

const char* VSD_FILE = "/dev/vsd";

int get_size() 
{
    int vsd_file = open(VSD_FILE, O_RDONLY);
    
    if(vsd_file < 0) 
    {
        printf("Can not open vsd device file\n");
        return EXIT_FAILURE;
    }

    vsd_ioctl_get_size_arg_t size;
    int ret = EXIT_SUCCESS;
    if(ioctl(vsd_file, VSD_IOCTL_GET_SIZE, &size) == -1)
    {
        printf("Can not get size from vsd device\n");
        ret = EXIT_FAILURE;
        goto out;
    }
    printf("Size = %zu\n", size.size);
out:
    close(vsd_file);
    return ret;
}

int set_size(size_t size) 
{
    int vsd_file = open(VSD_FILE, O_RDONLY);
    
    if(vsd_file < 0) 
    {
        printf("Can not open vsd device file\n");
        return EXIT_FAILURE;
    } 
    vsd_ioctl_set_size_arg_t dev_size;
    dev_size.size = size;
    int ret = EXIT_SUCCESS;
    if(ioctl(vsd_file, VSD_IOCTL_SET_SIZE, &dev_size) == -1) 
    {
        printf("Can not set size on vsd device\n");
        ret = EXIT_FAILURE;
    }
    else
    {
        printf("Size was successful set to %zu\n", size);
    }
    close(vsd_file);
    return ret;
}

int main(int argc, char **argv) 
{
    
    if(argc < 2) 
        goto usage;

    if(!strcmp(argv[1], "size_get")) 
    {
        if(argc != 2) 
            goto usage;
        return get_size();
    }

    if(!strcmp(argv[1], "size_set")) 
    {
        if(argc != 3)
            goto usage;
        int size = strtoll(argv[2], NULL, 10);
        if(errno || size < 0) 
            goto usage;
        return set_size((size_t)size);
    }

usage:
    printf("Invalid size or type of arguments\n\nUsage: vsd_userspace size_set <SIZE_IN_BYTES> | size_get \n");
    return EXIT_FAILURE;
}
