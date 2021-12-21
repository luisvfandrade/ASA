/*
Problema1(X)
    if |X| == 0
        return
    let LIS be an array of size |X|
    LIS[0] = 1
    let maxLIS = 1
    let nLIS = 1
    for i = 1 to |X| - 1
        let j = i - 1
        while X[j] > X[i]
            j--
        LIS[i] = LIS[J] + 1
        if LIS[i] > maxLIS
            maxLIX = LIS[i]
            nLIS = 1
        else if LIS[i] == maxLIS
            nLIS = nLIS + 1
    return maxLIS, nLIS
*/