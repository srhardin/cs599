// set the dimensions and margins of the graph
var margin = {top: 20, right: 20, bottom: 50, left: 80},
    width = 600  - margin.right,
    height = 400 - margin.top - margin.bottom;

// append the hist object to the body of the page
// append a 'group' element to 'hist'
// moves the 'group' element to the top left margin
var hist = d3.select("#histogram").append("svg")
    .attr("width", width + margin.left)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

function BuildGainsHistogram()
{
    // Build the ranges
    var max = d3.max(gains_losses);
    var min = d3.min(gains_losses);
    
    // Bin the data
    var histGenerator = d3.histogram()
    .domain([min,max])
    .thresholds(20);
    
    var bins = histGenerator(gains_losses);

    // set the ranges
    var x = d3.scaleLinear()
        .domain([min, max])
        .range([0, width]);
    
    var yMax = d3.max(bins, function(d){return d.length});
    var yMin = d3.min(bins, function(d){return d.length});
        
    var y = d3.scaleLinear()
        .domain([0, yMax])
        .range([height, 0]);
    
    var color = d3.scaleOrdinal().range(["#AA0000", "#00AA00"]);
    
    var bar = hist.selectAll(".bar")
        .data(bins)
        .enter().append("rect")
         //.attr("class", "bar")
        .attr("x", function(d) { return x(d.x0); })
        .attr("width", 12)
        .attr("y", function(d) { return y(d.length); })
        .attr("height", function(d) { return height - y(d.length); })
        .style("fill", function(d, i) 
        { 
            if(d.x0 < 0.0) {
                return color(0);
            }
            else {
                return color(1)
            }
        }); // IMPORTANT!!! <-- SAM

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
      .text("Gain and Loss Percents");

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
      .text("Number in the Bucket"); 
}

function ResetHistogram()
{
    hist.remove();
}