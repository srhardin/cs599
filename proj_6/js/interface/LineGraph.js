var margin = {top: 50, right: 50, bottom: 50, left: 50}
width = 600;
height = 400;

var idx = 0;

function AfterLoadLine()
{
    //// Monstrosity. Nasty. Bleh!!!
    //var agedData = d3.nest()
    //.key(function(d) 
    //{
    //    return d.min_age;
    //    })
    //.rollup(function(leaves) 
    //{
    //    return d3.median(leaves, function(d) 
    //        {
    //            return +d.average_rating;
    //        });
    //})
    //.sortKeys((a,b) => d3.ascending(+a, +b))
    //.entries(csvData);
    
    var xScale = d3.scaleLinear()
    .domain([0, 252]) // input
    .range([0, width]); // output

    var yScale = d3.scaleLinear()
    .domain([d3.min(value_over_time) - 10000, d3.max(value_over_time) + 10000]) // input 
    .range([height, 0]); // output 

    // 7. d3's line generator
    var line = d3.line()
    .x(function(d) { return xScale(idx++); }) // set the x values for the line generator
    .y(function(d) { return yScale(d); }) // set the y values for the line generator 

    // 1. Add the SVG to the page and employ
    var svg = d3.select("#chart").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    // 3. Call the x axis in a group tag
    svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(xScale)); // Create an axis component with d3.axisBottom
    
    // text label for the y axis
    svg.append("text")
      .attr("y", (height + margin.bottom - 25))
      .attr("x", (width/2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Date");

    // 4. Call the y axis in a group tag
    svg.append("g")
    .attr("class", "y axis")
    .call(d3.axisLeft(yScale)); // Create an axis component with d3.axisLeft
    
    // text label for the y axis
    svg.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 - margin.left)
      .attr("x", 0 - (height / 2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Account Value");

    // 9. Append the path, bind the data, and call the line generator 
    svg.append("path")
    .datum(value_over_time) // 10. Binds data to the line 
    .attr("class", "line") // Assign a class for styling 
    .attr("d", line); // 11. Calls the line generator 
}