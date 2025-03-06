function [label,myScore] = scores2label(scores,classes)
    if scores(1) > scores(2)
        label = classes(1);
        myScore = scores(1);
    else
        label = classes(2);
        myScore = scores(2);
    end