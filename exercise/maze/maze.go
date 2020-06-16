package main

import (
	"fmt"
	"os"
)

func readMaze(name string) [][]int {
	file, err := os.Open(name)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	var row, col int
	_, err = fmt.Fscanf(file, "%d %d\r\n", &row, &col)
	if err != nil {
		panic(err)
	}
	maze := make([][]int, row)
	for i := range maze {
		maze[i] = make([]int, col)
		for j := range maze[i] {
			if j != col - 1 {
				_, err = fmt.Fscanf(file, "%d", &maze[i][j])
			} else {
				_, err = fmt.Fscanf(file, "%d\r\n", &maze[i][j])
			}
			if err != nil {
				panic(err)
			}
		}
	}
	return maze
}

type point struct {
	i, j int
}

var dirs = [4]point{
		{-1, 0},
		{0, -1},
		{1, 0},
		{0, 1},
}

func (p point) add(r point) point {
	return point{p.i + r.i, p.j + r.j}
}

func walkMaze(maze [][]int, start, end point) [][]int {
	steps := make([][]int, len(maze))
	for i, _ := range steps {
		steps[i] = make([]int, len(maze[0]))
	}

	var queue []point
	queue = append(queue, start)
	steps[start.i][start.j] = 0

	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]

		if cur.i == end.i && cur.j == end.j {
			break
		}

		for i, _ := range dirs {
			check := cur.add(dirs[i])
			if check.i < 0 || check.i >= len(maze) {
				continue
			}
			if check.j < 0 || check.j >= len(maze[0]) {
				continue
			}
			if maze[check.i][check.j] != 0 {
				continue
			}
			if check.i == 0 && check.j == 0 || steps[check.i][check.j] != 0 {
				continue
			}

			steps[check.i][check.j] = steps[cur.i][cur.j] + 1
			queue = append(queue, check)
		}
	}

	return steps
}

func printMatrix(m [][]int, name string) {
	fmt.Println(name)
	for _, row := range m {
		for _, val := range row{
			fmt.Printf("%2d ", val)
		}
		fmt.Println()
	}
}

func printPath(steps [][]int, end point) {
	var path []point
	cur := end
	for cur.i != 0 || cur.j != 0 {
		path = append(path, cur)
		for i := range dirs {
			check := cur.add(dirs[i])
			if check.i < 0 || check.i >= len(steps) {
				continue
			}
			if check.j < 0 || check.j >= len(steps[0]) {
				continue
			}
			if steps[check.i][check.j] + 1 == steps[cur.i][cur.j] {
				cur = check
				break
			}
		}
	}
	path = append(path, point{0, 0})

	for idx := len(path) - 1; idx >= 0; idx-- {
		fmt.Printf("(%d, %d)", path[idx].i, path[idx].j)
	}
	fmt.Println()
}

func main() {
	maze := readMaze("maze/maze.txt")
	printMatrix(maze, "walking maze:")

	steps := walkMaze(maze, point{0, 0},
			point{len(maze) - 1, len(maze[0]) - 1})
	printMatrix(steps, "all steps list:")
	printPath(steps, point{len(maze) - 1, len(maze[0]) - 1})

	steps = walkMaze(maze, point{0, 0},
			point{2, 2})
	printMatrix(steps, "{2,2} steps list:")
	printPath(steps, point{2, 2})
}
