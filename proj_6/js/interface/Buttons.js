function StartSimulation()
{
    g_StartDate = document.getElementById('startDate').value;
    g_EndDate = document.getElementById('endDate').value;
    
    run_simulation();
    
    // Update the visuals
    UpdateGraphs();
    SetAllStats();
}

function ResetGraphs()
{
    ResetLineGraph();
    ResetHistogram();
}

function UpdateGraphs()
{
    avGraph.update(value_over_time);
}

const avGraph = new AccountValueGraph();