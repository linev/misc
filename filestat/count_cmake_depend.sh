#!/bin/bash

find . -name 'depend.make' -type f -printf '%b.%f\0' |

  awk -F . -v RS='\0' '

    {s[$NF] += $1; n[$NF]++}

    END {for (e in s) printf "%15d %4d %s\n", s[e]*512, n[e], e}' |

  sort -n