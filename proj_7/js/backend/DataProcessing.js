/*
    DataProcessing.js
    Author: Sawyer Goodsel & Samuel Hardin
    Class: cs399f19
    Assignment: Project 7
    Copyright 2019 DigiPen Institute of Technology
*/

function getData()
{
    d3.csv("https://raw.githubusercontent.com/srhardin/cs599/master/proj_7/data/data.csv").then(function(data)
    {
        for (var i = 0; i < data.length; i++) 
        {
            data[i].fileName                = data[i].fileName.substring(11);
            
            data[i].code                    = +data[i].code;
            data[i].comments                = +data[i].comments;
            data[i].includes                = +data[i].includes;
            data[i].defines                 = +data[i].defines;
            data[i].pragmas                 = +data[i].pragmas;
            data[i].define_conditionals     = +data[i].define_conditionals;
            data[i].preprocess_conditionals = +data[i].preprocess_conditionals;
            data[i].types                   = +data[i].types;
            data[i].conditionals            = +data[i].conditionals;
            data[i].loops                   = +data[i].loops;
            data[i].structures              = +data[i].structures;
            data[i].members                 = +data[i].members;
            data[i].namespaces              = +data[i].namespaces;
            data[i].casts                   = +data[i].casts;
            data[i].memorys                 = +data[i].memorys;
            data[i].exceptions              = +data[i].exceptions;
            data[i].miscs                   = +data[i].miscs;
            data[i].asms                    = +data[i].asms;
            data[i].gotos                   = +data[i].gotos;
            data[i].mutables                = +data[i].mutables;
            data[i].unions                  = +data[i].unions;
            
            console.log(data[i]);
        }
        
        console.log("Done loading data...");
        UpdateGraphs();
    });
}


