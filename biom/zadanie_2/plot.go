package main

import (
	"encoding/csv"
	"log"
	"os"
	"strconv"

	"github.com/gonum/plot"
	"github.com/gonum/plot/plotter"
	"github.com/gonum/plot/plotutil"
	"github.com/gonum/plot/vg"
)

func main() {

	r := csv.NewReader(os.Stdin)
	r.Comma = ' '
	records, _ := r.ReadAll()

	p, err := plot.New()
	if err != nil {
		panic(err)
	}

	p.Title.Text = "Úspešnosť klasifikácie v %"
	p.X.Label.Text = "Počet trénovacích vzoriek"
	p.Y.Label.Text = "Úspešnosť klasifikácie"

	err = plotutil.AddLinePoints(p,
		"Bayes", points(records, 0, 1),
		"Svm", points(records, 2, 3),
	)
	if err != nil {
		panic(err)
	}
	// Save the plot to a PNG file.
	if err := p.Save(8*vg.Inch, 8*vg.Inch, "classification.png"); err != nil {
		panic(err)
	}
}

func points(rows [][]string, x, y int) plotter.XYs {
	var (
		err    error
		points = make(plotter.XYs, len(rows))
	)
	for i, row := range rows {
		if points[i].X, err = strconv.ParseFloat(row[x], 64); err != nil {
			log.Fatal(err)
		}
		if points[i].Y, err = strconv.ParseFloat(row[y], 64); err != nil {
			log.Fatal(err)
		}
	}
	return points
}
