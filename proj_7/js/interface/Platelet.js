var tempdata = [];
class PlateletGraph
{
    constructor()
    {
        this.margin = {top: 50, right: 50, bottom: 50, left: 50};
        this.width = 350;
        this.height = 350;
        this.values = [{key: "comments"                , val: 0},
                       {key: "includes"                , val: 0},
                       {key: "defines"                 , val: 0},
                       {key: "pragmas"                 , val: 0},
                       {key: "define_conditionals"     , val: 0},
                       {key: "preprocess_conditionals" , val: 0},
                       {key: "types"                   , val: 0},
                       {key: "conditionals"            , val: 0},
                       {key: "loops"                   , val: 0},
                       {key: "structures"              , val: 0},
                       {key: "members"                 , val: 0},
                       {key: "namespaces"              , val: 0},
                       {key: "casts"                   , val: 0},
                       {key: "memorys"                 , val: 0},
                       {key: "exceptions"              , val: 0},
                       {key: "miscs"                   , val: 0},
                       {key: "asms"                    , val: 0},
                       {key: "gotos"                   , val: 0},
                       {key: "mutables"                , val: 0},
                       {key: "unions"                  , val: 0}];
                       
        this.tooltip = {};
        
        this.initialize();
    }
    
    initialize()
    {
        var color = d3.scaleOrdinal(d3.schemeCategory10);
        var radius = Math.min(this.width-150,this.height-150)/2;
        this.arc = d3.arc()
            .outerRadius(radius)
            .innerRadius(2)
            .cornerRadius(40);

        var a=this.width/2;
        var b=this.height/2 - 50;
        this.svg = d3.select("#platelet").append("svg")
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
            .value(function(d){return d.val;})
            .padAngle(.02);
            
        var g = this.svg.selectAll(".arc")
            .data(pie(this.values))
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
                var others = g.filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 0.6);
                
                tooltip
                    .style("left", (d3.event.pageX+12) + "px")
                    .style("top", (d3.event.pageY-10) + "px")
                    .html("<b>" + d.data.key + ": </b>"+ d.data.val);
            })
            .on("mouseout",function()
            {
                var current = this;
                var others = g.filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 1.0);
                
                return tooltip.style("visibility", "hidden");
            });
        
        g.append("path")
            .attr("d",this.arc)
            .style("fill",function(d){return color(d.data.val);});
    }

    update(data)
    {
        // TODO: Accumulate indices into the data
        this.values = [];
        var color = d3.scaleOrdinal(d3.schemeCategory10);
        
        this.svg.selectAll(".arc").remove();
        
        Object.entries(data).forEach(([key, value]) => {
            if(key != "fileName" && key != "code") {
                var temp = {key: key, val: value};
                this.values.push(temp);
            }
        });
        
        var tooltip = d3.select("body")
            .append("div")
            .attr("class", "tooltip")
            .style("z-index", "10")
            .text("a simple tooltip");
        
        var pie = d3.pie()
            .sort(null)
            .value(function(d){return d.val;})
            .padAngle(.02);
            
        var g = this.svg.selectAll(".arc")
            .data(pie(this.values))
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
                var others = g.filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 0.6);
                
                tooltip
                    .style("left", (d3.event.pageX+12) + "px")
                    .style("top", (d3.event.pageY-10) + "px")
                    .html("<b>" + d.data.key + ": </b>"+ d.data.val);
            })
            .on("mouseout",function()
            {
                var current = this;
                var others = g.filter(function(el) {
                    return this != current
                });
                others.selectAll("path").style('opacity', 1.0);
                
                return tooltip.style("visibility", "hidden");
            });
        
        g.append("path")
            .attr("d",this.arc)
            .style("fill",function(d){return color(d.data.val);});
    }
};