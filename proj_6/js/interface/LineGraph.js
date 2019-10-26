var margin = {top: 50, right: 50, bottom: 50, left: 50}
width = 600;
height = 400;

var idx = 0;

class AccountValueGraph
{
    constructor()
    {
        this.margin = {top: 50, right: 50, bottom: 50, left: 50};
        this.width = 600;
        this.height = 400;
        this.idx = 0;
        
        this.initialize();
    }
    
    initialize()
    {
        // Create the 
        var xScale = d3.scaleLinear()
            .domain([0, 252])
            .range([0, width]);
        
        var yScale = d3.scaleLinear()
            .domain([0, 100000])
            .range([height, 0]);
        
        // Set up the line generator
        this.line = d3.line()
            .x(function(d) { return xScale(idx++); })
            .y(function(d) { return yScale(d); })
        
        this.svg = d3.select("#accountValue")
            .append("svg")
            .attr("width", width + margin.left + margin.right)
            .attr("height", height + margin.top + margin.bottom)
            .append("g")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
            
            // 3. Call the x axis in a group tag
        this.svg.append("g")
            .attr("class", "x-axis")
            .attr("transform", "translate(0," + height + ")")
            .call(d3.axisBottom(xScale)); // Create an axis component with d3.axisBottom
        
        // text label for the y axis
        this.svg.append("text")
            .attr("y", (height + margin.bottom - 25))
            .attr("x", (width/2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Date");
        
        // 4. Call the y axis in a group tag
        this.svg.append("g")
            .attr("class", "y-axis")
            .call(d3.axisLeft(yScale)); // Create an axis component with d3.axisLeft
        
        // text label for the y axis
        this.svg.append("text")
            .attr("transform", "rotate(-90)")
            .attr("y", 0 - margin.left)
            .attr("x", 0 - (height / 2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Account Value");
    }
    
    update(newData)
    {
        idx = 0;
        
        // Update the scale values
        var xScale = d3.scaleLinear()
            .domain(DateHash[g_StartDate], DateHash[g_EndDate])
            .range([0, width]);
        
        var yScale = d3.scaleLinear()
            .domain([d3.min(newData) - 10000, d3.max(newData) + 10000])
            .range([height, 0]);
            
        this.svg.select("g")
            .attr("class", "x-axis")
            .call(d3.axisBottom(xScale));
            
        this.svg.select("g")
            .attr("class", "y-axis")
            .call(d3.axisLeft(yScale));
            
        // Call the line generator 
        this.svg.append("path")
            .datum(newData)
            .attr("class", "line")
            .attr("d", this.line);
    }
};

function BuildLineGraph()
{
    var xScale = d3.scaleLinear()
    .domain([0, 252]) // input
    .range([0, width]); // output

    var yScale = d3.scaleLinear()
    .domain([d3.min(value_over_time) - 10000, d3.max(value_over_time) + 10000]) // input 
    .range([height, 0]); // output 

    // 7. d3's line generator
    this.line = d3.line()
    .x(function(d) { return xScale(idx++); }) // set the x values for the line generator
    .y(function(d) { return yScale(d); }) // set the y values for the line generator 

    // 1. Add the SVG to the page and employ
    //if(ds.select("#chart").select("svg")
    var svg = d3.select("#accountValue")
    .append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    // 3. Call the x axis in a group tag
    svg.append("g")
    .attr("class", "x-axis")
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
    .attr("class", "y-axis")
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
    .datum(value_over_time)
    .attr("class", "line")
    .attr("d", line);
}

function ResetLineGraph()
{
    d3.select("#chart").exit().remove();
}