function StartSimulation()
{
    g_StartDate = document.getElementById('startDate').value;
    g_EndDate = document.getElementById('endDate').value;
    
    var alg = document.getElementById('algorithm');
    var opt = alg.options[alg.selectedIndex].innerHTML;
    
    if(opt == "one")
    {
        run_simulation();
    }
    else if (opt == "two")
    {
    }
    else if (opt == "three")
    {
    }
    else if (opt == "four")
    {
    }
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function UpdateGraphs()
{
    avGraph.update(value_over_time);
    lgGraph.update(gains_losses);
}

const avGraph = new AccountValueGraph();
const lgGraph = new GainsLossesHistogram();