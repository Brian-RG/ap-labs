package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	matrix:=make([][]uint8,dy)
	for i:=0;i<len(matrix);i++{
		matrix[i] =make([]uint8,dx)
		for j:=0;j<len(matrix[i]);j++{
			matrix[i][j]=uint8(i*j)
		}
	}
	
	return matrix
}

func main() {
	pic.Show(Pic)
}