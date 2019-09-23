package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)
// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	System_info:=[5]int{0,0,0,0,0} //Directories, other files, symbolic links, devices and sockets
	err:= filepath.Walk(dir,func(dir string, fi os.FileInfo, err error) error{
		fi,err=os.Lstat(dir)
		System_info[filetype(fi)]+=1 //filetype returns the number of the position of each type in the array, so we add 1
		return err
	})
	m:=strings.Repeat("-",len(dir)+2)
	fmt.Println("\nDirectory Scanner Tool")
	fmt.Println("+-------------------------+"+m+"+")
	fmt.Println("| Path                    |",dir+" |")
	fmt.Println("+-------------------------+"+m+"+")
	fmt.Println("| Directories             | ",System_info[0]," |")
	fmt.Println("| Symbolic Links          | ",System_info[2]," |")
	fmt.Println("| Devices                 | ",System_info[3]," |")
	fmt.Println("| Sockets                 | ",System_info[4]," |")
	fmt.Println("| Other files             | ",System_info[1]," |")
	fmt.Println("+-------------------------+"+m+"+")
	return err
}

func filetype(fi os.FileInfo) int{
	if fi.IsDir(){ //Checks if it's a directory
		return 0
	}
	mode:=fi.Mode()
	if (mode & os.ModeSymlink != 0) { //Checks if the file is a Symlink
		return 2
	}
	if (mode & os.ModeDevice !=0 ){	//Checks if it's a device
		return 3
	}
	if (mode & os.ModeSocket !=0){ //Checks if it's a socket
		return 4
	}
	return 1	//Returns as "other file"
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
	scanDir(os.Args[1])
}