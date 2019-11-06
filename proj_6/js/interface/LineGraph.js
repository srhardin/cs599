var tempdata = [];
class AccountValueGraph
{
    constructor()
    {
        this.margin = {top: 50, right: 50, bottom: 50, left: 50};
        this.width = 500;
        this.height = 300;
        this.idx = 0;
		this.data = [];
        
        this.initialize();
    }
    
    initialize()
    {
		// Set the ranges
        this.xScale = d3.scaleLinear().domain([0, 100]).range([0, this.width-this.margin.left/2]);
        this.yScale = d3.scaleLinear().domain([0, 100000]).range([this.height, 0]);
		
		var x = this.xScale;
		var tooltip = d3.select("#accountValue")
			.append("div")
			.style("opacity", 0)
			.attr("class", "tooltip")
			.style("background-color","white")
			.style("border","solid")
			.style("border-width","2px")
			.style("border-radius","2px")
			.style("padding","2px");
			
		var mouseover = function(d)
		{
			tooltip
			.style("opacity", 1)
			.attr("x", (d3.mouse(this)[0]+70) + "px")
			.attr("y", (d3.mouse(this)[1]+50) + "px")
			d3.select(this)
			.style("stroke","steel-blue")
			.style("opacity", 0.5);
			console.log("working");
		}
		
		var mousemove = function(d)
		{
			
			var x0 = d3.mouse(this)[0]
			//console.log(x0);
			console.log(d3.mouse(this)[0]);
			
			    //i = bisectDate(data, x0, 1),
                //d0 = data[i - 1],
                //d1 = data[i],
				
			var x1 = Math.floor(((x0 - 25) / 475) * tempdata.length);
			let tempdisplay = x1;
			x1 += date_hash[g_StartDate];
			var correctkey;
			
			for(var key in date_hash) 
			{
				if(date_hash[key] == x1)
				{
					correctkey = key;
					break;
				}
				
			}
			
			let display = correctkey + ", day: "+tempdisplay + " AccountValue: $" + Math.floor(tempdata[x1 - date_hash[g_StartDate]]);
				
			tooltip
				.html(display)
				.style("left", (d3.mouse(this)[0] + 10) + "px")
				.style("top", (d3.mouse(this)[1] + 50) + "px")
				.style("width", 300 + "px")
				.style("position", "relative")
				.style("z-index", "100");
				
			//console.log(x1);
			//console.log(d0);
			//console.log(d1);
			//console.log(correctkey);
		}
		
		var mouseout = function(d)
		{
			tooltip
			.style("opacity", 0)
			d3.select(this)
			.style("stroke","#ffab00")
			.style("opacity", 1.0);
		}

        
        // Define the axes
        this.xAxis = d3.axisBottom(this.xScale);
        this.yAxis = d3.axisLeft(this.yScale);
    
        // Define the line
        this.valueline = d3.line()
            .x(d => { return this.xFunc(); })
            .y(d => { return this.yFunc(d); });
        // Adds the svg canvas
        var svg = d3.select("#accountValue")
            .append("svg")
            .attr("width", this.width + this.margin.left + this.margin.right)
            .attr("height", this.height + this.margin.top + this.margin.bottom)
			.call(d3.zoom().on("zoom", function () 
				{
					svg.attr("transform", d3.event.transform);
					console.log("working");
				})
			)
            .append("g")
            .attr("transform", "translate(" + (this.margin.left) + "," + (this.margin.top) + ")");
			
		this.svg = svg;
        // Add the valueline path.
        this.svg.append("path")
            .attr("class", "line")
            .attr("d", this.valueline({}))
			.on("mouseover", mouseover)
			.on("mousemove", mousemove)
			.on("mouseout", mouseout);
    
        // Add the X Axis
        this.svg.append("g")
            .attr("class", "x axis")
            .attr("transform", "translate("+(this.margin.left/2)+"," + (this.height) + ")")
            .call(this.xAxis);
            
        // text label for the y axis
        this.svg.append("text")
            .attr("y", (-this.margin.bottom/2))
            .attr("x", (this.width/2))
            .attr("dy", "1em")
            .style("text-anchor", "middle")
            .text("Total Value");
    
        // Add the Y Axis
        this.svg.append("g")
            .attr("class", "y axis")
            .attr("transform", "translate(" + (this.margin.left/2) + ","+(0)+")")
            .call(this.yAxis);

            
        // text label for the y axis
        //this.svg.append("text")
        //    .attr("transform", "rotate(-90)")
        //    .attr("y", - this.margin.left)
        //    .attr("x", - (this.height / 2))
        //    .attr("dy", "1em")
        //    .style("text-anchor", "middle")
        //    .text("Account Value");
    }

    update(data)
    {
		tempdata = data;
        this.idx = 0;
        
        // Scale the range of the data again 
        this.xScale.domain([0, data.length]);
        this.yScale.domain([d3.min(data) - 5000, d3.max(data) + 5000]);
    
        // Select the section we want to apply our changes to
        var svg = d3.select("#accountValue").transition();
    
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
        return this.xScale(this.idx++) + this.margin.left/2;
    }

    yFunc(d)
    {
        return this.yScale(d);
    }
};