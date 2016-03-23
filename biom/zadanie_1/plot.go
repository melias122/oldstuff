package main

import (
	"encoding/csv"
	"os"
	"strconv"

	"github.com/gonum/plot"
	"github.com/gonum/plot/plotter"
	"github.com/gonum/plot/plotutil"
	"github.com/gonum/plot/vg"
)

func main() {

	r := csv.NewReader(os.Stdin)
	r.Comma = ';'
	records, _ := r.ReadAll()

	p, err := plot.New()
	if err != nil {
		panic(err)
	}

	p.Title.Text = "FMR FNMR"
	p.X.Label.Text = "Threshold"
	p.Y.Label.Text = "%"

	err = plotutil.AddLinePoints(p,
		"FMR", g(records[1:], 1),
		"FNMR", g(records[1:], 2),
	)
	if err != nil {
		panic(err)
	}
	// Save the plot to a PNG file.
	if err := p.Save(8*vg.Inch, 8*vg.Inch, "fmr_fnmr.png"); err != nil {
		panic(err)
	}

	p2, err := plot.New()
	if err != nil {
		panic(err)
	}

	p2.Title.Text = "ROC"
	p.X.Label.Text = "FP"
	p.Y.Label.Text = "TP"

	err = plotutil.AddLinePoints(p2,
		"ROC", g2(records[1:], 4, 3),
		"Random", mid(len(records[1:])),
	)
	if err != nil {
		panic(err)
	}
	// Save the plot to a PNG file.
	if err := p2.Save(8*vg.Inch, 8*vg.Inch, "roc.png"); err != nil {
		panic(err)
	}
}

func mid(i int) plotter.XYs {
	p := make(plotter.XYs, i+1)

	var f float64
	for j := range p {
		p[j].X = f
		p[j].Y = f
		f += float64(1. / float64(i))
	}
	return p
}

func g2(r [][]string, i1, i2 int) plotter.XYs {
	p := make(plotter.XYs, len(r))
	for i := range r {
		x, _ := strconv.ParseFloat(r[i][i1], 64)
		y, _ := strconv.ParseFloat(r[i][i2], 64)
		p[i].X = x
		p[i].Y = y
	}
	return p
}

func g(r [][]string, idx int) plotter.XYs {
	p := make(plotter.XYs, len(r))
	for i := range r {
		x, _ := strconv.ParseFloat(r[i][0], 64)
		y, _ := strconv.ParseFloat(r[i][idx], 64)
		p[i].X = x
		p[i].Y = y
	}
	return p
}
