class StockValueGraph
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
        // Set the ranges
        this.xScale = d3.scaleLinear().domain([0, 100]).range([0, this.width-this.margin.left]);
        this.yScale = d3.scaleLinear().domain([0, 100000]).range([this.height, 0]);
        
        // Define the axes
        this.xAxis = d3.axisBottom(this.xScale);
        this.yAxis = d3.axisLeft(this.yScale);
    
        // Define the line
        this.valueline = d3.line()
            .x(d => { return this.xFunc(); })
            .y(d => { return this.yFunc(d); });
        
        // Adds the svg canvas
        this.svg = d3.select("#stockValue")
            .append("svg")
            .attr("width", this.width + this.margin.left + this.margin.right)
            .attr("height", this.height + this.margin.top + this.margin.bottom)
            .append("g")
            .attr("transform", "translate(" + (this.margin.left) + "," + (this.margin.top) + ")");
    
        // Add the valueline path.
        this.svg.append("path")
            .attr("class", "line")
            .attr("d", this.valueline({}));
    
        // Add the X Axis
        this.svg.append("g")
            .attr("class", "x axis")
            .attr("transform", "translate("+(this.margin.left)+"," + (this.height) + ")")
            .call(this.xAxis);
            
        // text label for the y axis
        this.svg.append("text")
            .attr("y", (this.height + this.margin.bottom/2))
            .attr("x", (this.width/2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Trading Day");
    
        // Add the Y Axis
        this.svg.append("g")
            .attr("class", "y axis")
            .attr("transform", "translate(" + (this.margin.left) + ","+(0)+")")
            .call(this.yAxis);
            
        // text label for the y axis
        this.svg.append("text")
            .attr("transform", "rotate(-90)")
            .attr("y", - this.margin.left/2)
            .attr("x", - (this.height / 2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Stock Value");
    }

    update(data)
    {
        this.idx = 0;
        
        // Scale the range of the data again 
        this.xScale.domain([0, data.length]);
        this.yScale.domain([d3.min(data) - d3.min(data)/2, d3.max(data) + 10]);
    
        // Select the section we want to apply our changes to
        var svg = d3.select("#stockValue").transition();
    
        // Make the changes
        svg.select(".line")   // change the line
            .duration(750)
            .attr("d", this.valueline(data));
        svg.select(".x.axis") // change the x axis
            .duration(750)
            .call(this.xAxis);
        svg.select(".y.axis") // change the y axis
            .duration(750)
            .call(this.yAxis);
    }
    
    xFunc()
    {
        return this.xScale(this.idx++) + this.margin.left;
    }

    yFunc(d)
    {
        return this.yScale(d);
    }
};


class StockOwnedGraph
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
        // Set the ranges
        this.xScale = d3.scaleLinear().domain([0, 100]).range([0, this.width-this.margin.left]);
        this.yScale = d3.scaleLinear().domain([0, 100000]).range([this.height, 0]);
        
        // Define the axes
        this.xAxis = d3.axisBottom(this.xScale);
        this.yAxis = d3.axisLeft(this.yScale);
    
        // Define the line
        this.valueline = d3.line()
            .x(d => { return this.xFunc(); })
            .y(d => { return this.yFunc(d); });
        
        // Adds the svg canvas
        this.svg = d3.select("#stockOwned")
            .append("svg")
            .attr("width", this.width + this.margin.left + this.margin.right)
            .attr("height", this.height + this.margin.top + this.margin.bottom)
            .append("g")
            .attr("transform", "translate(" + (this.margin.left) + "," + (this.margin.top) + ")");
    
        // Add the valueline path.
        this.svg.append("path")
            .attr("class", "line")
            .attr("d", this.valueline({}));
    
        // Add the X Axis
        this.svg.append("g")
            .attr("class", "x axis")
            .attr("transform", "translate("+(this.margin.left)+"," + (this.height) + ")")
            .call(this.xAxis);
            
        // text label for the y axis
        this.svg.append("text")
            .attr("y", (this.height + this.margin.bottom/2))
            .attr("x", (this.width/2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Trading Day");
    
        // Add the Y Axis
        this.svg.append("g")
            .attr("class", "y axis")
            .attr("transform", "translate(" + (this.margin.left) + ","+(0)+")")
            .call(this.yAxis);
            
        // text label for the y axis
        this.svg.append("text")
            .attr("transform", "rotate(-90)")
            .attr("y", - this.margin.left/2)
            .attr("x", - (this.height / 2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Stock Amount Owned");
    }

    update(data)
    {
        this.idx = 0;
        
        // Scale the range of the data again 
        this.xScale.domain([0, data.length]);
        this.yScale.domain([d3.min(data) - d3.min(data)/2, d3.max(data) + 10]);
    
        // Select the section we want to apply our changes to
        var svg = d3.select("#stockOwned").transition();
    
        // Make the changes
        svg.select(".line")   // change the line
            .duration(750)
            .attr("d", this.valueline(data));
        svg.select(".x.axis") // change the x axis
            .duration(750)
            .call(this.xAxis);
        svg.select(".y.axis") // change the y axis
            .duration(750)
            .call(this.yAxis);
    }
    
    xFunc()
    {
        return this.xScale(this.idx++) + this.margin.left;
    }

    yFunc(d)
    {
        return this.yScale(d);
    }
};