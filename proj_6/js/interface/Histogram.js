// set the dimensions and margins of the graph
var margin = {top: 20, right: 20, bottom: 50, left: 80},
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

// set the ranges
var x = d3.scaleBand()
        .range([0, width])
        .padding(0.1);
var y = d3.scaleLinear()
        .range([height, 0]);

// append the hist object to the body of the page
// append a 'group' element to 'hist'
// moves the 'group' element to the top left margin
var hist = d3.select("#chart2").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

var g_data;
d3.csv("https://raw.githubusercontent.com/srhardin/cs599/master/board_games_clean01.csv")
	.then(function(data)
        {
            data.forEach(function(d) 
            {
                d.average_rating = +d.average_rating;
                d.users_rated = +d.users_rated;
            });
            
            g_data = data;
            AfterLoad();
        });
        
var nBin = 21;
function AfterLoad()
{
	//binning
    var nested_data = d3.nest().key(function(d) { return Math.round(d.average_rating * 2) / 2;})
    var means = nested_data
        .rollup(function(data_points) 
            {
                return d3.mean(data_points,
                function(d) 
                {
                    return d.users_rated;
                })
            })
        .sortKeys(d3.ascending)
        .entries(g_data);
        
    //end binning

    x.domain(means.map(function(d) { return d.key; }));
    y.domain([0, d3.max(means, function(d) { return d.value; })]);

    var color = d3.scaleOrdinal().range(["#6b486b", "#a05d56", "#d0743c", "#ff8c00"]);
    
    hist.selectAll(".bar")
        .data(means)
        .enter().append("rect")
         //.attr("class", "bar")
        .attr("x", function(d) { return x(d.key); })
        .attr("width", x.bandwidth())
        .attr("y", function(d) { return y(d.value); })
        .attr("height", function(d) { return height - y(d.value); })
        .style("fill", function(d, i) { return color(i%4); }); // IMPORTANT!!! <-- SAM

    // add the x Axis
    hist.append("g")
        .attr("transform", "translate(0," + height + ")")
        .call(d3.axisBottom(x));

    // text label for the x axis
    hist.append("text")             
      .attr("transform",
            "translate(" + (width/2) + " ," + 
                           (height + margin.bottom - 10) + ")")
      .style("text-anchor", "middle")
      .text("User rating (bins)");

    // add the y Axis
    hist.append("g")
        .call(d3.axisLeft(y));
        
  // text label for the y axis
  hist.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 - margin.left)
      .attr("x", 0 - (height / 2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Average Number of Users Rated"); 

};
