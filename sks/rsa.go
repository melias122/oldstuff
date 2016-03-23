package main

import (
	"errors"
	"math/big"
)

var (
	bigZero = big.NewInt(0)
	bigOne  = big.NewInt(1)
	bigTwo  = big.NewInt(2)
)

type PublicKey struct {
	N *big.Int // modulus N = p.q.r...
	E *big.Int // exponent E = 65537
}

type PrivateKey struct {
	PublicKey
	D      *big.Int   // exponent
	Primes []*big.Int // factors of N
	Qinv   *big.Int
}

func RSAGenerateKey(nprimes, bits int) (*PrivateKey, error) {

	switch bits {
	case 1024, 2048, 4096:

	default:
		return nil, errors.New("RSA: unsupported key size")
	}

	if nprimes < 2 {
		return nil, errors.New("RSA: must have 2 or more primes")
	}

	// ignore error
	rng, _ := NewLFSR(bits)

	k := new(PrivateKey)
	k.E = big.NewInt(int64(65537))
	k.Primes = make([]*big.Int, nprimes)

	for {
		// get primes
		bitsLeft := bits
		for i := range k.Primes {
			k.Primes[i] = rng.NextPrimeBits(bitsLeft / (nprimes - i))
			bitsLeft -= k.Primes[i].BitLen()
		}

		// compute n, phi
		n := new(big.Int).Set(bigOne)
		pminus1 := new(big.Int)
		phi := new(big.Int).Set(bigOne)
		for _, prime := range k.Primes {
			n.Mul(n, prime)            // n *= prime
			pminus1.Sub(prime, bigOne) // prime - 1
			phi.Mul(phi, pminus1)      // phi *= (prime-1)
		}

		// check bit len
		if n.BitLen() != bits {
			continue
		}

		k.D = new(big.Int)
		g := new(big.Int)
		y := new(big.Int)
		g.GCD(k.D, y, k.E, phi)

		if g.Cmp(bigOne) != 0 {
			continue
		}

		if k.D.Sign() < 0 {
			k.D.Add(k.D, phi)
		}
		k.N = n
		break
	}

	// for opengpg
	k.Qinv = new(big.Int).ModInverse(k.Primes[1], k.Primes[0])

	return k, nil
}

func RSAEncrypt(ct *big.Int, key *PublicKey, m *big.Int) *big.Int {
	return ct.Exp(m, key.E, key.N) // ct = m^E (mod N)
}

func RSADecrypt(pt *big.Int, key *PrivateKey, m *big.Int) *big.Int {
	return pt.Exp(m, key.D, key.N) // pt = m^D (mod N)
}
