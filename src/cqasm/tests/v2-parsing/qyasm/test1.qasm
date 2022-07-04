// First tests for QYASM
QYASM 1.0;














// Gate Set Tomography





// for
for(ampl=0; ampl<100; ampl++) x q;
for(;;) x q[0];

// for with block
for(ampl=0; ampl<100; ampl++) {
    x q[0];
    x q[0];
    x q[1] | y q[2];
}

/*
for(ampl=0; ampl<100; ampl+=1) {
    x(ampl);
}
*/


// if ... else
if(a<b)
    y q[1];
else
    x q[2];

// if
if(a>b)
    y q[2];

// while
while(a>=b)
    spec q[12];

// do ... while
do
    x q[3];
while(a==b);


// gate syndrome d1,d2,d3,a1,a2
// {
//   cx d1,a1; cx d2,a1;
//   cx d2,a2; cx d3,a2;
// }
// x q[0]; // error
// barrier q;
// syndrome q[0],q[1],q[2],a[0],a[1];
// measure a -> syn;
// if(syn==1) x q[0];
// if(syn==2) x q[2];
// if(syn==3) x q[1];
// measure q -> c;
