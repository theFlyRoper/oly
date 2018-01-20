use Digest::SHA3 qw(sha3_256_hex);

$data = Argo;
@frags = split(//, $data);

# all-at-once (Functional style)
$digest1 = sha3_256_hex($data);

# in-stages (OOP style)
$state = Digest::SHA3->new(256);
for (@frags) { $state->add($_) }
$digest2 = $state->hexdigest;

print "$digest1 \n";

print $digest1 eq $digest2 ?
"that's the ticket!\n" : "oops!\n";
