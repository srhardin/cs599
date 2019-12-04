/*
    DataProcessing.js
    Author: Sawyer Goodsel & Samuel Hardin
    Class: cs399f19
    Assignment: Project 7
    Copyright 2019 DigiPen Institute of Technology
*/

function getData()
{
d3.csv("https://raw.githubusercontent.com/srhardin/cs599/master/proj_7/data/data.csv").then(function(data)
    {
        for (var i = 0; i < data.length; i++) 
        {
            data[i].fileName                = data[i].fileName.substring(11);
            
            data[i].code                    = +data[i].code;
            data[i].comments                = +data[i].comments;
            data[i].includes                = +data[i].includes;
            data[i].defines                 = +data[i].defines;
            data[i].pragmas                 = +data[i].pragmas;
            data[i].define_conditionals     = +data[i].define_conditionals;
            data[i].preprocess_conditionals = +data[i].preprocess_conditionals;
            data[i].types                   = +data[i].types;
            data[i].conditionals            = +data[i].conditionals;
            data[i].loops                   = +data[i].loops;
            data[i].structures              = +data[i].structures;
            data[i].members                 = +data[i].members;
            data[i].namespaces              = +data[i].namespaces;
            data[i].casts                   = +data[i].casts;
            data[i].memorys                 = +data[i].memorys;
            data[i].exceptions              = +data[i].exceptions;
            data[i].miscs                   = +data[i].miscs;
            data[i].asms                    = +data[i].asms;
            data[i].gotos                   = +data[i].gotos;
            data[i].mutables                = +data[i].mutables;
            data[i].unions                  = +data[i].unions;
        }
        
        console.log("Done loading data...");
        UpdateGraphs();
		
				console.log(data);
		
		//something like this
		
		// set the dimensions and margins of the graph
		var margin = {top: 20, right: 20, bottom: 120, left: 40},
			width = 850 - margin.left - margin.right,
			height = 575 - margin.top - margin.bottom;
		
		// set the ranges
		var x = d3.scaleBand()
				.range([0, width])
				.padding(0.1);
		var y = d3.scaleLinear()
				.range([height, 0]);
				
		// append the svg object to the body of the page
		// append a 'group' element to 'svg'
		// moves the 'group' element to the top left margin
		var svg = d3.select("#accountValue").append("svg")
			.attr("width", width + margin.left + margin.right)
			.attr("height", height + margin.top + margin.bottom)
		.append("g")
			.attr("transform", 
				"translate(" + margin.left + "," + margin.top + ")");
		
		
		// Scale the range of the data in the domains
		x.domain(data.map(function(d) { return d.fileName; }));
		y.domain([0, d3.max(data, function(d) { return d.code; })]);
		
		// append the rectangles for the bar chart
		svg.selectAll(".bar")
			.data(data)
			.enter().append("rect")
			.attr("class", "bar")
			.attr("x", function(d) { return x(d.fileName); })
			.attr("width", x.bandwidth())
			.attr("y", function(d) { return y(d.code); })
			.attr("height", function(d) { return height - y(d.code); });
		
		// add the x Axis
		svg.append("g")
			.attr("transform", "translate(0," + (height) +  ")")
			.call(d3.axisBottom(x))
		.selectAll("text")
			.attr("y", 0)
			.attr("x", 9)
			.attr("dy", ".35em")
			.attr("transform", "rotate(90)")
			.style("text-anchor", "start");
		
		// add the y Axis
		svg.append("g")
			.call(d3.axisLeft(y));
    });
}


