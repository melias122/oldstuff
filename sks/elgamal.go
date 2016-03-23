package main

import (
	"errors"
	"math/big"
	"strings"
)

var (
	p1024, g1024, q1024 *big.Int
	p2048, g2048, q2048 *big.Int
)

// PublicKey for ElGamal
// P - prime
// G - group generator
// Y - public key value Y = G^X (mod P),  where X is secret
type PublicKeyEG struct {
	P, G, Y *big.Int
}

type PrivateKeyEG struct {
	PublicKeyEG
	X *big.Int
}

func ElGamalGenerateKey(nbits int) (pk *PrivateKeyEG, err error) {

	var (
		p, g, q *big.Int
	)

	switch nbits {
	case 1024:
		p = p1024
		g = g1024
		q = q1024
	case 2048:
		p = p2048
		g = g2048
		q = q2048
	default:
		return nil, errors.New("ElGamal: unsupported key size")
	}

	// choose rand x from <1, q-1>
	rng, _ := NewLFSR(nbits)
	var x *big.Int
	for {
		x = rng.RandMod(q)
		if x.Cmp(bigZero) > 0 {
			break
		}
	}
	pk = &PrivateKeyEG{
		PublicKeyEG: PublicKeyEG{
			P: new(big.Int).Set(p),
			G: new(big.Int).Set(g),
			Y: new(big.Int).Exp(g, x, p),
		},
		X: x,
	}
	return
}

func ElGamalEncrypt(pub *PublicKeyEG, m *big.Int) (c1 *big.Int, c2 *big.Int) {

	rng, _ := NewLFSR(1024)
	k := rng.RandMod(pub.P)

	c1 = new(big.Int).Exp(pub.G, k, pub.P)  // c1 = G^k mod P
	ss := new(big.Int).Exp(pub.Y, k, pub.P) // ss = Y^k mod P (shared secret)
	c2 = ss.Mul(ss, m)                      // c2 = ss * m
	c2.Mod(c2, pub.P)                       // c2 = c2 mod P

	return
}

func ElGamalDecrypt(priv *PrivateKeyEG, c1, c2 *big.Int) (m *big.Int) {

	ss := new(big.Int).Exp(c1, priv.X, priv.P) // ss = c1^X mod P
	ss.ModInverse(ss, priv.P)                  // ss = ss^-1 mod P
	m = ss.Mul(ss, c2)                         // m = ss * c2
	m.Mod(m, priv.P)                           // m = m mod P

	return
}

// hexes from https://tools.ietf.org/html/rfc5114#section-2.1
func init() {

	// 1024-bit MODP Group with 160-bit Prime Order Subgroup
	p1024hex := `B10B8F96 A080E01D DE92DE5E AE5D54EC 52C99FBC FB06A3C6
       9A6A9DCA 52D23B61 6073E286 75A23D18 9838EF1E 2EE652C0
       13ECB4AE A9061123 24975C3C D49B83BF ACCBDD7D 90C4BD70
       98488E9C 219A7372 4EFFD6FA E5644738 FAA31A4F F55BCCC0
       A151AF5F 0DC8B4BD 45BF37DF 365C1A65 E68CFDA7 6D4DA708
       DF1FB2BC 2E4A4371`

	g1024hex := `A4D1CBD5 C3FD3412 6765A442 EFB99905 F8104DD2 58AC507F
       D6406CFF 14266D31 266FEA1E 5C41564B 777E690F 5504F213
       160217B4 B01B886A 5E91547F 9E2749F4 D7FBD7D3 B9A92EE1
       909D0D22 63F80A76 A6A24C08 7A091F53 1DBF0A01 69B6A28A
       D662A4D1 8E73AFA3 2D779D59 18D08BC8 858F4DCE F97C2A24
       855E6EEB 22B3B2E5`

	q1024hex := `F518AA87 81A8DF27 8ABA4E7D 64B7CB9D 49462353`

	// 2048-bit MODP Group with 256-bit Prime Order Subgroup
	p2048hex := `87A8E61D B4B6663C FFBBD19C 65195999 8CEEF608 660DD0F2
		5D2CEED4 435E3B00 E00DF8F1 D61957D4 FAF7DF45 61B2AA30
		16C3D911 34096FAA 3BF4296D 830E9A7C 209E0C64 97517ABD
		5A8A9D30 6BCF67ED 91F9E672 5B4758C0 22E0B1EF 4275BF7B
		6C5BFC11 D45F9088 B941F54E B1E59BB8 BC39A0BF 12307F5C
		4FDB70C5 81B23F76 B63ACAE1 CAA6B790 2D525267 35488A0E
		F13C6D9A 51BFA4AB 3AD83477 96524D8E F6A167B5 A41825D9
		67E144E5 14056425 1CCACB83 E6B486F6 B3CA3F79 71506026
		C0B857F6 89962856 DED4010A BD0BE621 C3A3960A 54E710C3
		75F26375 D7014103 A4B54330 C198AF12 6116D227 6E11715F
		693877FA D7EF09CA DB094AE9 1E1A1597`

	g2048hex := `3FB32C9B 73134D0B 2E775066 60EDBD48 4CA7B18F 21EF2054
       07F4793A 1A0BA125 10DBC150 77BE463F FF4FED4A AC0BB555
       BE3A6C1B 0C6B47B1 BC3773BF 7E8C6F62 901228F8 C28CBB18
       A55AE313 41000A65 0196F931 C77A57F2 DDF463E5 E9EC144B
       777DE62A AAB8A862 8AC376D2 82D6ED38 64E67982 428EBC83
       1D14348F 6F2F9193 B5045AF2 767164E1 DFC967C1 FB3F2E55
       A4BD1BFF E83B9C80 D052B985 D182EA0A DB2A3B73 13D3FE14
       C8484B1E 052588B9 B7D2BBD2 DF016199 ECD06E15 57CD0915
       B3353BBB 64E0EC37 7FD02837 0DF92B52 C7891428 CDC67EB6
       184B523D 1DB246C3 2F630784 90F00EF8 D647D148 D4795451
       5E2327CF EF98C582 664B4C0F 6CC41659`

	q2048hex := `8CF83642 A709A097 B4479976 40129DA2 99B1A47D 1EB3750B
       A308B0FE 64F5FBD3`

	var (
		replacer = strings.NewReplacer(" ", "", "\n", "", "\t", "")
		ok       bool
	)

	for _, s := range []struct {
		hex string
		b   **big.Int
	}{
		{p1024hex, &p1024},
		{g1024hex, &g1024},
		{q1024hex, &q1024},

		{p2048hex, &p2048},
		{g2048hex, &g2048},
		{q2048hex, &q2048},
	} {
		*s.b, ok = new(big.Int).SetString(replacer.Replace(s.hex), 16)
		if !ok {
			panic("ElGamal: failed to parse hex strings")
		}
	}
}
