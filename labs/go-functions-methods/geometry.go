package main

import (
	"fmt"
	"math"
	"math/rand"
	"strconv"
	"time"
	"os"
)

type Point struct{ x,y float64}

// getter of x
func (p Point) X() float64 {
	return p.x
}

// getter of y
func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Round(math.Hypot(q.x-p.x, q.y-p.y)*100) / 100
}

// generate a random float in range -100, 100
func randomNumber()float64{
    rand.Seed(time.Now().UnixNano())
    return math.Round(((rand.Float64()*200)-100)*100) / 100
}

func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)
	if (o1 != o2) && (o3 != o4) {
		return true
	}
	if (o1 == 0) && onSegment(p1, p2, q1) {
		return true
	}
	if (o2 == 0) && onSegment(p1, q2, q1) {
		return true
	}
	if (o3 == 0) && onSegment(p2, p1, q2) {
		return true
	}
	if (o4 == 0) && onSegment(p2, q1, q2) {
		return true
	}
	return false
}

func onSegment(p, q, r Point) bool {
	if (q.X() <= math.Max(p.X(), r.X())) && (q.X() >= math.Min(p.X(), r.X())) && (q.Y() <= math.Max(q.Y(), r.Y())) && (q.Y() >= math.Min(p.Y(), r.Y())) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 {
		return 0
	}
	if val > 0 {
		return 1
	}
	return 2
}

// A Path is a journey connecting the sides with straight sides.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	distance := 0.0
	for i := range path {
		if i > 0 {
			distance += path[i-1].Distance(path[i])
		}
	}
	return math.Round(distance*100) / 100
}

func (path Path) intersection() bool{
  intersect := false
	for i := 0; i < len(path)-3; i++ {
		for j := 0; j < i+1; j++ {
			intersect = doIntersect(path[j], path[j+1], path[len(path)-2], path[len(path)-1])
		}
	}
	return intersect
}

func getPerimeter(sides int, path Path){
  fmt.Println("- Figure's Perimeter")
  fmt.Print("- ")
	perimeter := 0.0
	i := 0
	for i < sides{
		//in case is the last point
	    if i == sides-1 {
	    	perimeter = perimeter + path[i].Distance(path[(i+1)%sides])
	      	fmt.Printf(" %.2f =", path[i].Distance(path[(i+1)%sides]))
	      	fmt.Printf(" %.2f\n", perimeter)
	      	return
	    }
	    perimeter = perimeter + path[i].Distance(path[(i+1)%sides])
	    fmt.Printf("%.2f + ", path[i].Distance(path[(i+1)%sides]))
	    i++
	}
    
} 

func main() {
    sides, _ := strconv.Atoi(os.Args[1])
    //validation
    if(sides<3){
        fmt.Println("You need at least 3 sides to make a figure")
        return 
    }
    fmt.Printf("- Generating a [%d] sides figure", sides)
    fmt.Println("\n- Figure's vertices")
    figure := Path{}
    for i := 0; i < sides; i++ { 
    	figure = append(figure, Point{randomNumber(), randomNumber()})
    	//if finds an intersection
		for figure.intersection() {
			figure[i] = Point{randomNumber(), randomNumber()}
		}
		fmt.Printf("- ( %.1f, %.1f)\n", figure[i].X(), figure[i].Y())
	}
	getPerimeter(sides, figure)
}