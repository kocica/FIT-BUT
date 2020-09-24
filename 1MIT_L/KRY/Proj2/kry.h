// ---------------------------------------------------------------------------
// @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
// @brief  KRY - Project 2 - RSA Implementation
// @date   22.03.2020
// @file   kry.h
// ---------------------------------------------------------------------------

#include <random>
#include <iostream>

// MPZ
#include <gmpxx.h>

class IRSA_t
{
    // Input hex numbers are passed as string and mpz is directly initialized from these strings.
    public:
        virtual void generate( const std::string& B )                                             = 0;
        virtual void encrypt( const std::string& E, const std::string& N, const std::string& M )  = 0;
        virtual void decrypt( const std::string& D, const std::string& N, const std::string& C )  = 0;
        virtual void breaking( const std::string& E, const std::string& N, const std::string& C ) = 0;
};

class MillerRabin_t
{
private:
    gmp_randclass *prng{ 0 };

public:
    mpz_class powMod( mpz_class a, mpz_class x, const mpz_class& n )
    {
        mpz_class r = 1;

        while( x > 0 )
        {
            if( ( x & 1 ) == 1 )
            {
                r = a * r % n;
            }
            x >>= 1;
            a = a * a % n;
        }

        return r;
    }

    void initSeed( const size_t bytes )
    {
        if( !prng )
        {
            prng = new gmp_randclass( gmp_randinit_default );
        }

        if( bytes > 0 )
        {
            FILE *f = fopen( "/dev/urandom", "rb" );
            if( f )
            {
                mpz_class seed = 0;
                for( size_t i = 0; i < bytes; ++i )
                {
                    int n = fgetc( f );
                    seed = ( seed << 8 ) | n;
                }

                fclose( f );
                prng->seed( seed );
                return;
            }
        }

        prng->seed( time( NULL ) );
    }

    mpz_class intRand( const mpz_class& min, const mpz_class& max )
    {
        if( !prng )
        {
            initSeed( 32 );
        }

        return min == max ? min : prng->get_z_range( max - min + 1 ) + min;
    }

    bool isPrime( const mpz_class& n, const size_t rounds )
    {
        if( ( n & 1 ) == 0 )
        {
            return false;
        }

        size_t s = 0;
        {
            mpz_class m = n - 1;
            while( ( m & 1 ) == 0 )
            {
                ++s;
                m >>= 1;
            }
        }

        const mpz_class d = ( n - 1 ) / ( mpz_class( 1 ) << s );

        for( size_t i = 0; i < rounds; ++i )
        {
            const mpz_class a = intRand( 2, n - 2 );
            mpz_class x = powMod( a, d, n );

            if( x == 1 || x == ( n - 1 ) )
            {
                continue;
            }

            for( size_t r = 0; r < ( s - 1 ); ++r )
            {
                x = powMod( x, 2, n );
                if( x == 1 )
                {
                    return false;
                }
                if( x == n - 1 )
                {
                    break;
                }
            }

            if( x != ( n - 1 ) )
            {
                return false;
            }
        }

        return true;
    }
};

class RSA_t : IRSA_t
{
    private:
        gmp_randstate_t state;
        MillerRabin_t mr;

    public:
        RSA_t()
        {
            gmp_randinit_default( state );
            gmp_randseed_ui( state, static_cast<int32_t>( time( NULL ) ) );
        }

        void generate( const std::string& B )
        {
            // Declare and init GMP variables for calculation
            mpz_t N;   mpz_init( N );
            mpz_t E;   mpz_init( E );
            mpz_t P;   mpz_init( P );
            mpz_t Q;   mpz_init( Q );
            mpz_t D;   mpz_init( D );
            mpz_t G;   mpz_init( G );
            mpz_t Pm1; mpz_init( Pm1 );
            mpz_t Qm1; mpz_init( Qm1 );
            mpz_t Phi; mpz_init( Phi );

            // Calculate N
            int32_t reqLen = std::stoi( B ) / 2;
            mpz_class pp, qq;

            gmp_randclass rr( gmp_randinit_default );

            pp = rr.get_z_bits( reqLen );
            while( !mr.isPrime( pp, 25 ) )
            {
                pp = rr.get_z_bits( reqLen ) | ( mpz_class( 1 ) << ( reqLen - 1 ) );
            }

            qq = rr.get_z_bits( reqLen );
            while( !mr.isPrime( qq, 25 ) )
            {
                qq = rr.get_z_bits( reqLen ) | ( mpz_class( 1 ) << ( reqLen - 1 ) );
            }

            mpz_set( P, pp.get_mpz_t() );
            mpz_set( Q, qq.get_mpz_t() );

            mpz_mul( N, P, Q );

            //std::cout << "Size: " << mpz_sizeinbase( N, 2 ) << std::endl;

            // Calculate Phi(n)
            mpz_sub_ui( Qm1, Q, 1 );
            mpz_sub_ui( Pm1, P, 1 );
            mpz_mul( Phi, Pm1, Qm1 );

            // Calculate E
            do
            {
                mpz_t t; mpz_init( t );
                mpz_set_ui( t, 1 );
                randomPrime( E, t, Phi );
                gcd( G, E, Phi );
            }
            while( mpz_cmp_si( G, 1 ) != 0 );

            // Calculate multiplicative inverse
            Inverse( D, Phi, E );

            // Print results
            gmp_printf( "0x%Zx 0x%Zx 0x%Zx 0x%Zx 0x%Zx\n", P, Q, N, E, D );

            // Free resources
            mpz_clear( P );
            mpz_clear( Q );
            mpz_clear( N );
            mpz_clear( E );
            mpz_clear( D );
            mpz_clear( Pm1 );
            mpz_clear( Qm1 );
            mpz_clear( Phi );
        }

        void encrypt( const std::string& E, const std::string& N, const std::string& M )
        {
            mpz_t m, e, n;
            mpz_init( m );
            mpz_init( e );
            mpz_init( n );

            mpz_set_str( m, M.c_str(), 16 );
            mpz_set_str( e, E.c_str(), 16 );
            mpz_set_str( n, N.c_str(), 16 );

            mpz_powm( m, m, e, n );

            gmp_printf( "0x%Zx\n", m );
        }

        void decrypt( const std::string& D, const std::string& N, const std::string& C )
        {
            mpz_t d, n, c;
            mpz_init( d );
            mpz_init( n );
            mpz_init( c );

            mpz_set_str( d, D.c_str(), 16 );
            mpz_set_str( n, N.c_str(), 16 );
            mpz_set_str( c, C.c_str(), 16 );

            mpz_powm( c, c, d, n );

            gmp_printf( "0x%Zx\n", c );
        }

        void breaking( const std::string& E, const std::string& N, const std::string& C )
        {
            mpz_t e, n, c, p, q, d, p1, q1, phi;
            mpz_init( e );
            mpz_init( n );
            mpz_init( c );
            mpz_init( p );
            mpz_init( q );
            mpz_init( d );
            mpz_init( p1 );
            mpz_init( q1 );
            mpz_init( phi );

            mpz_set_str( e, E.c_str(), 16 );
            mpz_set_str( n, N.c_str(), 16 );
            mpz_set_str( c, C.c_str(), 16 );

            pollardRho( p, n );

            mpz_tdiv_q( q, n, p );
            mpz_sub_ui( q1, q, 1 );
            mpz_sub_ui( p1, p, 1 );
            mpz_mul( phi, p1, q1 );

            Inverse( d, phi, e );

            mpz_powm( c, c, d, n );

            gmp_printf( "0x%Zx 0x%Zx 0x%Zx\n", p, q, c );
        }

    protected:

        void randomPrime( mpz_t & prime, const std::string& minS, const std::string& maxS )
        {
            mpz_t min;  mpz_init( min );
            mpz_t max;  mpz_init( max );

            mpz_set_str( min, minS.c_str(), 16 );
            mpz_set_str( max, maxS.c_str(), 16 );

            randomPrime( prime, min, max );
        }

        void randomPrime( mpz_t & prime, const mpz_t& min, const mpz_t& max )
        {
            do
            {
                mpz_urandomm( prime, state, max );
            }
            while( mpz_cmp( prime, min ) < 0 || !mr.isPrime( mpz_class{ prime }, 25 ) );
        }

        void Update( mpz_t& a, mpz_t& b, const mpz_t& y )
        {
            mpz_t temp, temp2;
            mpz_init( temp );
            mpz_init( temp2 );

            // Backup 'b'
            mpz_set( temp, b );

            // Calculate a - y * temp
            mpz_mul( temp2, y, temp );
            mpz_sub( b, a, temp2 );

            // Set 'b' to 'a'
            mpz_set( a, temp );
        }

        // Inspired by: J. Nechvatal - Public-Key Cryptography (NIST SP 800-2)
        void Inverse( mpz_t& d, const mpz_t& n, const mpz_t& x )
        {
            mpz_t g, h, w, z, v, r, y;
            mpz_init( g );
            mpz_init( h );
            mpz_init( w );
            mpz_init( z );
            mpz_init( v );
            mpz_init( r );
            mpz_init( y );

            mpz_set( g, n );
            mpz_set( h, x );
            mpz_set_si( w, 1 );
            mpz_set_si( z, 0 );
            mpz_set_si( v, 0 );
            mpz_set_si( r, 1 );

            while( mpz_cmp_si( h, 0 ) > 0 )
            {
                mpz_tdiv_q( y, g, h );
                Update( g, h, y );
                Update( w, z, y );
                Update( v, r, y );
            }

            mpz_mod( d, v, n );
        }

        void gcd( mpz_t& g, const mpz_t& a, const mpz_t& b )
        {
            if( mpz_cmp_si( b, 0 ) == 0 )
            {
                mpz_set( g, a );
                return;
            }

            mpz_t t; mpz_init( t );
            mpz_mod( t, a, b );
            gcd( g, b, t );
        }

        mpz_class gcd_cpp( mpz_class a, mpz_class b )
        {
            mpz_class x = a, y = b;
            mpz_class gcd;

            while( y != 0 )
            {
                gcd = y;
                y = x % y;
                x = gcd;
            }

            return gcd;
        }

        void pollardRho( mpz_t& p, const mpz_t& n )
        {
            mpz_class x{ 0 }, y{ 0 }, c{ 0 }, d{ 1 }, N{ n };

            if( N == 1 )
            {
                mpz_set( p, N.get_mpz_t() );
                return;
            }


            if( N % 2 == 0 )
            {
                mpz_set_si( p, 2 );
                return;
            }

            x = mr.intRand( 1, N - 1 );
            c = mr.intRand( 1, N - 1 );
            y = x;

            while( d == 1 )
            {
                mpz_powm_ui( x.get_mpz_t(), x.get_mpz_t(), 2, N.get_mpz_t() ); 
                x = ( x + c + N ) % N;

                mpz_powm_ui( y.get_mpz_t(), y.get_mpz_t(), 2, N.get_mpz_t() ); 
                y = ( y + c + N ) % N;

                mpz_powm_ui( y.get_mpz_t(), y.get_mpz_t(), 2, N.get_mpz_t() ); 
                y = ( y + c + N ) % N;

                d = gcd_cpp( abs( x - y ), N );

                if ( d == N )
                {
                    pollardRho( p, n );
                    return;
                }
            }

            mpz_set( p, d.get_mpz_t() );
        }
};

enum class Mode_t
{
    ERROR,
    GENERATE,
    ENCRYPT,
    DECRYPT,
    BREAKING
};

struct Options_t
{
    explicit Options_t( Mode_t m ) : mode{ m } {}
    explicit Options_t( Mode_t m, const std::string& f ) : mode{ m }, first{ f } {}
    explicit Options_t( Mode_t m, const std::string& f, const std::string& s, const std::string& t )
                      : mode{ m }, first{ f }, second{ s }, third{ t } {}

    // Modes
    Mode_t mode;
    // Operands
    std::string first;
    std::string second;
    std::string third;

    Mode_t getMode() const { return mode; }
};

// Function to remove leading 0x from hex numbers as GMP seem to cannot handle this itself.
std::string clear( const std::string& in )
{
    static const std::string x = "0x";

    if ( in.rfind( x, 0 ) == 0 )
    {
        return in.substr( 2 );
    }

    return in;
}

Options_t parseInput( int argc, char **argv )
{
    if( argc < 2 )
    {
        return Options_t{ Mode_t::ERROR };
    }

    std::string option( argv[1] );

    try
    {
        if( option == "-g" )
        {
            if( argc < 3 ) return Options_t{ Mode_t::ERROR };
            return Options_t{ Mode_t::GENERATE, argv[2] };
        }
        else if( option == "-e" )
        {
            if( argc < 5 ) return Options_t{ Mode_t::ERROR };
            return Options_t{ Mode_t::ENCRYPT, clear( argv[2] ), clear( argv[3] ), clear( argv[4] ) };
        }
        else if( option == "-d" )
        {
            if( argc < 5 ) return Options_t{ Mode_t::ERROR };
            return Options_t{ Mode_t::DECRYPT, clear( argv[2] ), clear( argv[3] ), clear( argv[4] ) };
        }
        else if( option == "-b" )
        {
            if( argc < 5 ) return Options_t{ Mode_t::ERROR };
            return Options_t{ Mode_t::BREAKING, clear( argv[2] ), clear( argv[3] ), clear( argv[4] ) };
        }
    }
    catch( std::invalid_argument& )
    {
        // Fall-through
    }

    return Options_t{ Mode_t::ERROR };
}
