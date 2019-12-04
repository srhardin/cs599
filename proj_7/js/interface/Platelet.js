var tempdata = [];
class PlateletGraph
{
    constructor()
    {
        this.margin = {top: 50, right: 50, bottom: 50, left: 50};
        this.width = 350;
        this.height = 350;
        this.idx = 0;
		this.data = [];
        
        this.initialize();
    }
    
    initialize()
    {
        this.data = [{"age":"Below 6 years","population":511},
                     {"age":"6 yrs \- 12 yrs","population":394},
                     {"age":"12 yrs \- 16 yrs","population":429},
                     {"age":"16 yrs \- 18 yrs","population":568},
                     {"age":"18 yrs \- 30 yrs","population":13117},
                     {"age":"30 yrs \- 45 yrs","population":13094},
                     {"age":"45 yrs \- 60 yrs","population":5225},
                     {"age":"Above 60 yrs","population":1116}];
            
        var radius = Math.min(this.width-150,this.height-150)/2;
        var color = d3.scaleOrdinal(d3.schemeCategory10);
        var arc = d3.arc()  
            .outerRadius(radius)
            .innerRadius(2)
            .cornerRadius(40);

        var a=this.width/2;
        var b=this.height/2 - 40;
        var svg = d3.select("#platelet").append("svg")
            .attr("viewBox", "0 0 " + this.width + " " + this.height)
            .attr("preserveAspectRatio", "xMidYMid meet")
            .append("g")
            .attr("transform","translate("+a+","+b+")");

        var tooltip = d3.select("body")
            .append("div")
            .attr("class", "tooltip")
            .style("z-index", "10")
            .text("a simple tooltip");
    
        var pie = d3.pie()
            .sort(null)
            .value(function(d){return d.population;})
            .padAngle(.02);
            
        var g = svg.selectAll(".arc")
            .data(pie(this.data))
            .enter()
            .append("g")
            .attr("class","arc")
            .on("mouseover", function(d)
            {
                return tooltip.style("visibility", "visible");
            })
            .on("mousemove",function(d)
            {
                var current = this;
                var others = svg.selectAll(".arc").filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 0.6);
                
                tooltip
                    .style("left", (d3.event.pageX+12) + "px")
                    .style("top", (d3.event.pageY-10) + "px")
                    .html("<b>Age: </b>"+ d.data.age + "</br>" + "<b>No. of Victims: </b>" + d.data.population);
            })
            .on("mouseout",function()
            {
                var current = this;
                var others = svg.selectAll(".arc").filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 1.0);
                
                return tooltip.style("visibility", "hidden");
            });
        
        g.append("path")
            .attr("d",arc)
            .style("fill",function(d){return color(d.data.age);})
            .attr("d", arc);
    }

    update(data)
    {
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
};