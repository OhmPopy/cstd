
 double f__cabs(double, double);
static void z_sqrt(dcomplex *r, dcomplex *z)
{
        double mag, zi = z->i, zr = z->r;

        if( (mag = f__cabs(zr, zi)) == 0.)
                r->r = r->i = 0.;
        else if(zr > 0)
                {
                r->r = sqrt(0.5 * (mag + zr) );
                r->i = zi / r->r / 2;
                }
        else
                {
                r->i = sqrt(0.5 * (mag - zr) );
                if(zi < 0)
                        r->i = - r->i;
                r->r = zi / r->i / 2;
                }
        }

