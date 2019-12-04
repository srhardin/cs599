function StartSimulation()
{
    // ensure valid dates
    //GetValidDates();
    //g_StartMoney = +document.getElementById('startMoney').value;
    
    //var alg = document.getElementById('algorithm');
    //var opt = alg.options[alg.selectedIndex].innerHTML;
    //g_OwnedPerDay = [];
    //if(opt == "Mean Reversion")
    //{
    //    algorithm1();
    //}
    //else if (opt == "Largest Loss")
    //{
    //    algorithm2();
    //}
    //else if (opt == "Random")
    //{
    //    algorithm3();
    //}
    //else if (opt == "2nd Derivative")
    //{
    //    algorithm4();
    //}
    
    // Update the visuals
    UpdateGraphs();
    //SetAllStats();
    //changeTaxes(document.getElementById('taxBracket'));
}

function UpdateGraphs()
{
    //plateletGraph.update();
}

const plateletGraph = new PlateletGraph();