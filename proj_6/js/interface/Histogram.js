class GainsLossesHistogram
{
    constructor()
    {
        this.margin = {top: 50, right: 25, bottom: 50, left: 50};
        this.width = 500;
        this.height = 300;
        
        this.color = {red: "#870007", green: "#32D332"};
        
        this.initialize();
    }
    
    initialize()
    {
        this.xScale = d3.scaleLinear()
            .domain([-5, 5])
            .range([0, this.width]);
            
        this.yScale = d3.scaleLinear()
            .domain([0, 75])
            .range([this.height, 0]);
        
        // Define the axes
        this.xAxis = d3.axisBottom(this.xScale).ticks(10);
        this.yAxis = d3.axisLeft(this.yScale);
        
        this.svg = d3.select("#gainsLosses").append("svg")
            .attr("width", this.margin.right + this.width + this.margin.left)
            .attr("height", this.height + this.margin.top + this.margin.bottom)
            .append("g")
            .attr("transform", "translate(" + this.margin.left + "," + this.margin.top + ")");
            
        // add the x Axis
        this.svg.append("g")
            .attr("class", "x axis")
            .attr("transform", "translate("+ this.margin.left/6 +"," + this.height + ")")
            .call(this.xAxis);
    
        // text label for the x axis
        this.svg.append("text")             
            .attr("transform", "translate(" + (this.width/2) + " ," + -1 * this.margin.top/3  + ")")
            .style("text-anchor", "middle")
            .text("Gain and Loss Percents");
    
        // add the y Axis
        this.svg.append("g")
            .attr("transform", "translate("+ this.margin.left/6 +",0)")
            .attr("class", "y axis")
            .call(this.yAxis);
    }
    
    update(data)
    {
        // Find extents
        var max = d3.max(data);
        var min = d3.min(data);
        
        // Bin the data
        var histGenerator = d3.histogram()
            .domain([min,max])
            .thresholds(20);
            
        var bins = histGenerator(data);
        
        var yMax = d3.max(bins, function(d){return d.length});
        this.xScale.domain([min, max]);
        this.yScale.domain([0, yMax]);
        
        this.svg.selectAll("svg > g > rect").remove();
        
        this.svg.selectAll("bar")
            .data(bins)
            .enter().append("rect")
            .attr("x", d => { return this.xFunc(d.x0); })
            .attr("width", 20)
            .attr("y", d => { return this.yFunc(d.length); })
            .attr("height", d => { return this.height - this.yFunc(d.length); })
            .style("fill", d => 
            { 
                if(d.x0 < 0.0) {
                    return this.color.red;
                }
                else {
                    return this.color.green
                }
            });
            
        // Select the section we want to apply our changes to
        var changes = d3.select("#gainsLosses").transition();
    
        // Make the changes
        changes.select(".x.axis") // change the x axis
            .duration(750)
            .call(this.xAxis);
        changes.select(".y.axis") // change the y axis
            .duration(750)
            .call(this.yAxis);
    }
    
    xFunc(d)
    {
        return this.xScale(d) + this.margin.left/6;
    }

    yFunc(d)
    {
        return this.yScale(d);
    }
};