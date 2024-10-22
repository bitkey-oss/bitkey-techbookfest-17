
= 無料でデプロイして遊んじゃおう！

//flushright{
おーたかこーたろー

@otakakot
//}

== はじめに

「無料」

誰しも一度は聞いたことがある魔法の言葉です。
そんな「無料」に魅了され、Go でどうしても開発して遊びたいエンジニアの知見について共有します。

== 無料 × Go の選択肢

私がよく遊ぶのに使う無料で Go のサーバーを実装しデプロイできるサービスとして3つの選択肢があるので簡単にご紹介します。

=== Koyeb

Koyeb は Web アプリケーションや API のデプロイに特化したサーバレスプラットフォームを提供するサービスです。
1アカウント1つまで無料でコンテナをデプロイすることができます。
Go でサーバーを構築する方はコンテナにてデプロイすることが多いかと思います。
Koyeb はコンテナをそのままデプロイすることができるので特別なことは意識せず普段の開発体験と変わらず利用することが可能です。

=== Cloudflare

Cloudflare は CDN (Content Delivery Network ) やセキュリティ対策などを提供するサービスです。
エッジコンピューティングを活用した開発者向けのプラットフォームも提供しています。なかでも Cloudflare Workers という FaaS ( Function as a Service ) にて無料で Go のサーバーを構築することが可能です。
Cloudflare Workers は JavaScript, TypeScript もしくは Wasm ( Rust ) がメインの開発言語になります。
しかし、syumai さんが開発している workers というライブラリを使えば Go での開発が可能になります。
Cloudflare はほかにも制限はありますが無料で利用できるリソースが存在します。
Key Value ストアである Cloudflare Workers KV
Edge ロケーションで動作する SQLite である Cloudflare D1
AWS S3 と互換性がある Cloudflare R2
があります。
Cloudflare Workers とこれらリソースを組み合わせることでよりアプリケーション開発を楽しくことができます。
しかし、Cloudflare Workers は無料版だとデプロイサイズやリクエストあたりの実行時間に制限があるので注意が必要です。

=== Vercel

Vercel は Paas ( Platform as a Service ) を提供するサービスです。
Next.js という React のフレームワークを開発しているということから Webサイトホスティングをするイメージが強いですが、Vercel Functions というサービスを使えばサーバーサイドの構築も可能です。
また、Cloudflare のようにほかにも無料で利用できるリソースが存在しています。
RDB の利用が可能な Vercel Postgres
Key Value Store の利用が可能な Vercel KV
ファイルストレージの管理が可能な Vercel Blob
リードオンリーな設定などを管理可能な Vercel Edge Config
があります。
私はこの Vercel を使った無料開発がとくにお気に入りなので詳しく始め方についてご紹介します。

== Vercel × Go ではじめる Go サーバーの無料開発

=== 準備

開始するためにまず、Vercel アカウントを作成します。
無料で使うために Hobby 版を選択します。

また、各種操作を行うために Vercel CLI をインストールします。
いくつかインストールする方法はありますが npm にて導入します。

//list[npm][][shell]{
npm i -g vercel
//}

プロジェクトを開始するために Vercel にログインします。

//list[login][][shell]{
vercel login
//}

=== Functions のデプロイ

プロジェクトを作成しデプロイを実行します。

//list[ディレクトリ作成][][shell]{
mkdir <project>
cd <project>
go mod init <project>
mkdir api
touch api/index.go
//}

Functions のエントリーポイントとして以下のような Go 実装を用意します。

//list[Handler][][go]{
package api

import (
	"fmt"
	"net/http"
)

func Handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<h1>Hello from Go!</h1>")
}
//}

シンプルにアクセスが発生した場合に結果を返します。
また、Functions の設定を行うために以下の json ファイルを用意します。

//list[vercel.json作成][][shell]{
touch vercel.json
//}

//list[vercel.json][][json]{
{
  "build": {
    "env": {
      "GO_BUILD_FLAGS": "-ldflags '-s -w'"
    }
  }
}
//}

ここまで準備ができたら CLI を使ってデプロイします。

//list[prod][][shell]{
vercel --prod
//}

こんな感じで簡単に Go のサーバーを構築 & デプロイができてしまいます。

=== パスルーティング

Vercel Functions へデプロイしたサーバーへ対するアクセスは　vercel.json の routes を設定することで可能になります。

//list[vercel.json2][][json]{
{
    "routes": [
        {
            "src": "/",
            "dest": "/api",
            "methods": [
                "GET"
            ]
        }
    ]
}
//}

src で API としての設定を dest で対象の Go が実装されているファイルへのパスを設定します。
methods を指定することで許可するメソッドを設定できます。

=== Vercel Storage

Vercel Storage は Vercel KV, Vercel Postgres, Vercel Blob, Vercel Edge Config からなる Vercel でデータ永続化を行うことができるサービス群です。

Vercel KV は Key/Value と JSON データを扱えます。
Vercel Postgres は構造化されたリレーショナル・データ (SQL) を扱えます。
Vercel Blob は大容量ファイル （blobs） を扱えます。
Vercel Edge Config はリードオンリーなデータストアでランタイム用の設定 (feature flags) などを扱えます。

JavaScript (TypeScript) の場合は公式から各種 SDK が提供されているので簡単に扱うことが可能でが、Go には SDK の提供がありません。
それぞれのサービスにおいて Go からアクセスする方法を簡単にご紹介します。

※ Vercel Functions でなくてもそれぞれの Storage は触れるので今回はローカルマシンから各リソースへとアクセスしています。
※ 各種環境変数に設定している値は Vercel ダッシュボードにて Storage を作成したときに設定されているものです。

==== Vercel KV

Vercel KV は Redis の互換性があります。
Vercel KV でサポートされているコマンドは以下の@<href>{https://vercel.com/docs/storage/vercel-kv/redis-compatibility,公式サイト}から確認できます@<fn>{site}。

//footnote[site][https://vercel.com/docs/storage/vercel-kv/redis-compatibility]

Go からは redis/go-redis や redis/rueidis などのライブラリを利用することで操作が可能となります。

redis/go-redis を使ったサンプル実装を以下に記載します。

//list[main][][go]{
package main

import (
	"context"
	"crypto/tls"
	"os"
	"time"

	"github.com/redis/go-redis/v9"
)

func main() {
	address := os.Getenv("KV_URL")
	if address == "" {
		panic("KV_URL is required")
	}

	opt, err := redis.ParseURL(address)
	if err != nil {
		panic(err)
	}

	opt.TLSConfig = &tls.Config{
		MinVersion: tls.VersionTLS13,
	}

	cli := redis.NewClient(opt)

	if err := cli.Ping(context.Background()).Err(); err != nil {
		panic(err)
	}

	ctx := context.Background()

	if err := cli.Set(ctx, "key", "test", time.Minute).Err(); err != nil {
		panic(err)
	}

	val, err := cli.Get(ctx, "key").Result()
	if err != nil {
		panic(err)
	}

	println(val)

	if err := cli.Del(ctx, "key").Err(); err != nil {
		panic(err)
	}
}
//}

==== Vercel Postgres

Vercel Postgres はその名の通り PostgreSQL として扱うことができます。

Go からは lib/pq や jackc/pgx などのライブラリを利用することで操作が可能となります。

lib/pq を使ったサンプル実装を以下に記載します。

//list[main2][][go]{
package main

import (
	"context"
	"database/sql"
	"os"

	_ "github.com/lib/pq"
)

func main() {
	dsn := os.Getenv("POSTGRES_URL")
	if dsn == "" {
		panic("POSTGRES_URL is required")
	}

	db, err := sql.Open("postgres", dsn)
	if err != nil {
		panic(err)
	}

	defer func() {
		if err := db.Close(); err != nil {
			println(err)
		}
	}()

	ctx := context.Background()

	if err := db.PingContext(ctx); err != nil {
		panic(err)
	}

	rows, err := db.QueryContext(ctx, "SELECT 1")
	if err != nil {
		panic(err)
	}

	defer rows.Close()

	for rows.Next() {
		var one int
		if err := rows.Scan(&one); err != nil {
			panic(err)
		}

		println(one)
	}
}
//}

==== Vercel Blob

Vercel Blob についてはOSSミドルウェアなどとの互換性などはありません。

公式サイトに REST API でアクセスするための情報も記載されていなかったので公式が配布している TypeScript 製の vercel/storage を参考にして Go から HTTP にて操作することが可能となります。

以下のような実装でアップロード・メタデータの取得・ダウンロード・削除の操作が可能です。

//list[main3][][go]{
package main

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"time"

	"github.com/google/uuid"
)

const (
	BLOB_API_VERSION = "7" // ref: https://github.com/vercel/storage/blob/main/packages/blob/src/api.ts#L82
	DEFAULT_BASE_URL = "https://blob.vercel-storage.com"
)

func main() {
	ctx := context.Background()

	body := bytes.NewReader([]byte("test"))

	pathname := uuid.NewString() + ".txt"

	put, err := Put(ctx, pathname, body)
	if err != nil {
		panic(err)
	}

	fmt.Printf("response: %+v\n", put)

	head, err := Head(ctx, put.URL)
	if err != nil {
		panic(err)
	}

	fmt.Printf("head: %+v\n", head)

	download, err := Download(ctx, put.DownloadURL)
	if err != nil {
		panic(err)
	}

	fmt.Printf("download: %s\n", download)

	if err := Del(ctx, put.URL); err != nil {
		panic(err)
	}
}

type PutResponse struct {
	URL                string `json:"url"`
	DownloadURL        string `json:"downloadUrl"`
	Pathname           string `json:"pathname"`
	ContentDisposition string `json:"contentDisposition"`
}

func Put(
	ctx context.Context,
	pathname string,
	body io.Reader,
) (*PutResponse, error) {
	if pathname == "" {
		return nil, fmt.Errorf("pathname is required")
	}

	token := os.Getenv("BLOB_READ_WRITE_TOKEN")
	if token == "" {
		return nil, fmt.Errorf("BLOB_READ_WRITE_TOKEN is required")
	}

	base, _ := url.Parse(DEFAULT_BASE_URL)

	base.Path = pathname

	req, err := http.NewRequestWithContext(ctx, http.MethodPut, base.String(), body)
	if err != nil {
		return nil, fmt.Errorf("failed to create request: %w", err)
	}

	req.Header.Set("x-api-version", BLOB_API_VERSION)

	req.Header.Set("Authorization", "Bearer "+token)

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, fmt.Errorf("failed to send request: %w", err)
	}

	defer func() {
		if err := res.Body.Close(); err != nil {
			println(err)
		}
	}()

	println(res.StatusCode)

	var putRes PutResponse

	if err := json.NewDecoder(res.Body).Decode(&putRes); err != nil {
		return nil, fmt.Errorf("failed to decode response body: %w", err)
	}

	return &putRes, nil
}

type HeadResponse struct {
	HasMore bool       `json:"hasMore"`
	Blobs   []HeadBlob `json:"blobs"`
}

type HeadBlob struct {
	URL         string    `json:"url"`
	DownloadURL string    `json:"downloadUrl"`
	Pathname    string    `json:"pathname"`
	Size        int       `json:"size"`
	UploadedAt  time.Time `json:"uploadedAt"`
}

func Head(
	ctx context.Context,
	url string,
) (*HeadResponse, error) {
	token := os.Getenv("BLOB_READ_WRITE_TOKEN")
	if token == "" {
		return nil, fmt.Errorf("BLOB_READ_WRITE_TOKEN is required")
	}

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, DEFAULT_BASE_URL+"?"+url, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to create request: %w", err)
	}

	req.Header.Set("x-api-version", BLOB_API_VERSION)

	req.Header.Set("Authorization", "Bearer "+token)

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, fmt.Errorf("failed to send request: %w", err)
	}

	defer func() {
		if err := res.Body.Close(); err != nil {
			println(err)
		}
	}()

	println(res.StatusCode)

	var headRes HeadResponse

	if err := json.NewDecoder(res.Body).Decode(&headRes); err != nil {
		return nil, fmt.Errorf("failed to decode response body: %w", err)
	}

	return &headRes, nil
}

func Download(
	ctx context.Context,
	url string,
) ([]byte, error) {
	token := os.Getenv("BLOB_READ_WRITE_TOKEN")
	if token == "" {
		return nil, fmt.Errorf("BLOB_READ_WRITE_TOKEN is required")
	}

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to create request: %w", err)
	}

	req.Header.Set("x-api-version", BLOB_API_VERSION)

	req.Header.Set("Authorization", "Bearer "+token)

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, fmt.Errorf("failed to send request: %w", err)
	}

	defer func() {
		if err := res.Body.Close(); err != nil {
			println(err)
		}
	}()

	println(res.StatusCode)

	return io.ReadAll(res.Body)
}

type DelRequest struct {
	URLs []string `json:"urls"`
}
//}

==== Vercel Edge Config

Vercel Edge Config については@<href>{https://vercel.com/docs/storage/edge-config/vercel-api,公式サイト}に REST API にてアクセスする方法が記載されていました@<fn>{site2}。

//footnote[site2][https://vercel.com/docs/storage/vercel-kv/redis-compatibility]

こちらも Blob 同様に Go から HTTP にて操作が可能となります。

以下のような実装で値の取得・全件取得が可能です。

//list[main4][][go]{
package main

import (
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
)

const (
	DEFAULT_BASE_URL = "https://edge-config.vercel.com"
)

func main() {
	ctx := context.Background()

	value, err := Get(ctx, "test")
	if err != nil {
		panic(err)
	}

	fmt.Printf("value: %s\n", value)

	all, err := GetAll(ctx)
	if err != nil {
		panic(err)
	}

	fmt.Printf("all: %v\n", all)
}

func Get(
	ctx context.Context,
	key string,
) (string, error) {
	id := os.Getenv("EDGE_CONFIG_ID")
	if id == "" {
		return "", fmt.Errorf("EDGE_CONFIG_ID is required")
	}

	token := os.Getenv("EDGE_CONFIG_TOKEN")
	if token == "" {
		return "", fmt.Errorf("EDGE_CONFIG_TOKEN is required")
	}

	req, err := http.NewRequest(http.MethodGet, DEFAULT_BASE_URL+"/"+id+"/item/"+key+"?token="+token, nil)
	if err != nil {
		return "", err
	}

	req.Header.Set("Authorization", "Bearer "+token)

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return "", err
	}

	defer func() {
		if err := res.Body.Close(); err != nil {
			println(err)
		}
	}()

	println(res.StatusCode)

	var value string

	if err := json.NewDecoder(res.Body).Decode(&value); err != nil {
		return "", err
	}

	return value, nil
}

func GetAll(
	ctx context.Context,
) (map[string]string, error) {
	id := os.Getenv("EDGE_CONFIG_ID")
	if id == "" {
		return nil, fmt.Errorf("EDGE_CONFIG_ID is required")
	}

	token := os.Getenv("EDGE_CONFIG_TOKEN")
	if token == "" {
		return nil, fmt.Errorf("EDGE_CONFIG_TOKEN is required")
	}

	req, err := http.NewRequest(http.MethodGet, DEFAULT_BASE_URL+"/"+id+"/items?token="+token, nil)
	if err != nil {
		return nil, err
	}

	req.Header.Set("Authorization", "Bearer "+token)

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}

	defer func() {
		if err := res.Body.Close(); err != nil {
			println(err)
		}
	}()

	println(res.StatusCode)

	var all map[string]string

	if err := json.NewDecoder(res.Body).Decode(&all); err != nil {
		return nil, err
	}

	return all, nil
}
//}

== おわりに

会社名義の本であるにも関わらず完全趣味な話を投稿してしまいすみません。
ただ、ビットキーにどんな人がいるのかという紹介ができたという点においてはあながち大きく外れていないのかなと思い執筆させていただきました。

無料やGoにこだわり向き合うことでドキュメントを読み込んでみよう。OSSコードを読み込んでみよう。制約の中どうしよう などなど業務でも使えるようなエンジニア力も培えるのではないかと私は感じています。

みなさんもぜひ無料にこだわって遊んでみてください。
