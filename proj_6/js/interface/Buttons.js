function StartSimulation()
{
    // ensure valid dates
    GetValidDates();
    g_StartMoney = +document.getElementById('startMoney').value;
    
    var alg = document.getElementById('algorithm');
    var opt = alg.options[alg.selectedIndex].innerHTML;
    g_OwnedPerDay = [];
    if(opt == "Mean Reversion")
    {
        algorithm1();
    }
    else if (opt == "Largest Loss")
    {
        algorithm2();
    }
    else if (opt == "Random")
    {
        algorithm3();
    }
    else if (opt == "2nd Derivative")
    {
        algorithm4();
    }
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function GetValidDates()
{
    g_StartDate = undefined;
    g_EndDate = undefined;
    document.getElementById('startDay').innerHTML = "";
    document.getElementById('endDay').innerHTML = "";
    
    var tempStart = document.getElementById('startDate').value;
    var tempEnd = document.getElementById('endDate').value;
    var startDate = new Date(tempStart);
    var endDate = new Date(tempEnd);
    
    if(DateHash[tempStart] != undefined)
    {
        g_StartDate = tempStart;
    }
    
    if(DateHash[tempEnd] != undefined)
    {
        g_EndDate = tempEnd;
    }
    
    if(g_StartDate != undefined && g_EndDate != undefined)
    {
        return;
    }
    
    for(var date in DateHash)
    {
        var checkDate = new Date(date);
        
        if(g_StartDate == undefined)
        {
            if(checkDate > startDate)
            {
                g_StartDate = checkDate.toISOString().split('T')[0];
            }
        }
        
        if(g_EndDate != DateHash[tempEnd] || g_EndDate == undefined)
        {
            if(checkDate < endDate)
            {
                g_EndDate = checkDate.toISOString().split('T')[0];
            }
        }
    }
    
    document.getElementById('startDay').innerHTML = g_StartDate;
    document.getElementById('endDay').innerHTML = g_EndDate;
}

function UpdateGraphs()
{
    avGraph.update(value_over_time);
    stkvlGraph.update(getStockData(g_SelectedStock, g_StartDate, g_EndDate));
    stkcntGraph.update(getOwnedData(g_SelectedStock));
    lgGraph.update(gains_losses);

}

function UpdateStockGraphs(selectedStock)
{
    stkvlGraph.update(getStockData(selectedStock, g_StartDate, g_EndDate));
    stkcntGraph.update(getOwnedData());
}

const avGraph = new AccountValueGraph();
const stkvlGraph = new StockValueGraph();
const stkcntGraph = new StockOwnedGraph();
const lgGraph = new GainsLossesHistogram();