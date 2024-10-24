
= ViteでDevとProdの違いに沼ったことから得た学び

//flushright{
古澤 優也
//}

== まとめ

 * バージョン5.1未満のViteでは、CommonJSライブラリをnamespace importした際、DevとProdで挙動が異なる可能性があるという問題があった
 * Devモードだけで開発を行うのではなくProdの動作もpreviewコマンド等を使ってしっかり確認する必要がある
 * tsconfigの設定を適切に行うことも大事

== はじめに

以前Vite(v5.0.x)を用いてReact開発を行っている際、<b>{vite dev}で立ち上げたDevモードでは動作するが実際にビルド&デプロイしたProdモードで画面が白飛びしてしまう、という問題が発生したのでその原因を調べました。

== そもそもViteとは

Viteはフロントエンド向けビルドツールです。2020年に登場し2024年現在ではフロントエンド領域で幅広く使われるようになっています。GitHubのstar数で見ると、ビルドツールとしてこれまで広く利用されてきたwebpackを追い越しています。

特徴としては高速な開発サーバー立ち上げと高速なHMRです。ブラウザ上で直接実行できるというESModuleの特性をフル活用し、webpackのように全体をバンドルしてから開発サーバを立ち上げるのではなく、まず開発サーバを立ち上げその後モジュールを読み込んでいくことで高速な開発サーバ起動を実現します。また、事前バンドルという過程で外部パッケージ等をesbuildでESModule形式にビルドしておきそれをブラウザ上で積極的にキャッシュすることも行います。

== 発生した問題

React + Vite(5.0系)で次のようなコードを書いた際、Devモードでは問題なく動作したものの、デプロイしたものを確認したら<b>{xx is not function}というエラーが発生し画面が白飛びしてしまいました。どうやらビルド成果物を実行した際に関数でない何かしらのオブジェクトが関数呼び出しされていることが原因のようです。

//list[Component][][TypeScript]{
import * as dayjs from "dayjs";

const Component = () => {
  ...
  return (
    <>
        ...
        dayjs("2024-01-01")
      ...
    </>
　)
}
//}

以下の点がポイントです。

 * dayjs（CommonJS形式）をnamespace import（<b>{import *}）している
 * dayjsを関数として呼び出している

原因自体はシンプルで、namespace objectである<b>{dayjs}を関数呼び出ししてるのが問題です。ESModuleにおいてはnamespace importするとモジュール全体が一つのオブジェクトとしてimportされます。objectですのでもちろん関数呼び出しをすることができません。Day.jsの公式ドキュメントで（特定のtsconfigの設定がない限り）namespace importをするように記載があったのでそれに従ったのですが、誤りだったようです。

=== 問題の解決策

解決策としてはnamespace importをやめてdefault importにすればOKです。以下のようにコードを修正します。

//list[Component2][][TypeScript]{
import dayjs from "dayjs";
// 以下同じ
//}

default importをすることによって、<b>{module.exports}を通じてexportされたものを直接importすることができます。Dayjsの場合、<b>{dayjs}関数が直接exportされているので、default importすることによってそれをそのまま関数として利用できます。

もしくはnamespace importのままで<b>{dayjs.default("2024-01-01")}と呼び出す形でもOKです（この場合<b>{dayjs}はオブジェクトであり、<b>{default}というプロパティが<b>{dayjs}関数にあたる）。

=== tsconfigの設定でそもそも防げた

そもそもIDEツール上でエラーを出すためにはtsconfig.jsonの設定を以下のようにすべきでした。

//list[tsconfig.json][][json]{
{
	"compilerOptions": {
		...
		"esModuleInterop": true,
		...
	}
}
//}

<b>{esModuleInterop}はESModuleとCommonJSの互換性を確保するためのオプションです。これを有効にするとトランスパイル時にヘルパー関数を挿入しCommonJSをESModuleに適合した形（namespace importをobjectとしてimportする）でimportすることができるようになります。この設定をすることで、namespace importしたオブジェクトを関数呼び出しすると、IDE上で型エラーをキャッチすることができます。

ちなみに<b>{esModuleInterop}が<b>{true}であれば自動的に<b>{allowSyntheticDefaultImports}というオプションも自動的に有効化されます。このオプションははdefault exportが存在しないモジュールからのdefault importを許可するオプションで、CommonJSモジュールのdefault importが型チェック上可能になります。

以上の通り、原因自体はシンプルでそりゃそうだなという感じなのですが、ここで問題なのは **なぜDevでは正常に動きProdではエラーとなったのか？** です。

== なぜDevとProdで挙動が変わったのか？

ViteがDevモードにおいてnamespace importをdefault importに変換していることに起因します。CommonJS形式でありdefault exportされているdayjsメソッドを直接default importしているためエラーが発生しません。

実際にDev Toolを見てみると、以下のようにnamespace importがdefault importになっていることが分かります。

//image[image][Dev Toolの出力]{

//}

次にVite内部のコードを見てみます。以下の通りnamespace importがdefault importに明示的に書き換えられています。

//list[cjsModuleName][][TypeScript]{
const cjsModuleName = makeLegalIdentifier(
	`__vite__cjsImport${importIndex}_${rawUrl}`,
)
const lines: string[] = [`import ${cjsModuleName} from "${url}"`]
importNames.forEach(({ importedName, localName }) => {
	if (importedName === '*') {
	  lines.push(`const ${localName} = ${cjsModuleName}`)
	} else if (importedName === 'default') {
	  lines.push(
	    `const ${localName} = ${cjsModuleName}.__esModule ? 
                              ${cjsModuleName}.default : ${cjsModuleName}`,
    )
  } else {
	  lines.push(`const ${localName} = 
                              ${cjsModuleName}["${importedName}"]`)
  }
})
//}


@<href>{https://github.com/vitejs/vite/blob/80b1b07915295a32a7251df543509f8f667c5771/packages/vite/src/node/plugins/importAnalysis.ts#L1001, vitejs/vite}@<fn>{vitejs}

//footnote[vitejs][packages/vite/src/node/plugins/importAnalysis.ts]


前述の通りDayjsは<b>{dayjs}メソッドを直接exportしているため、importしたdayjs()関数を直接実行できてしまいます。

//list[dayjs][][TypeScript]{
import __vite__cjsImport6_dayjs 
  from "/node_modules/.vite/deps/dayjs.js?v=a45d0170";
const dayjs = __vite__cjsImport6_dayjs
//}

ViteのDevモードではこのようなimportの変換を行っているため問題なく実行することができます。しかしプロダクションビルドではこのような変換を行わないません。そのため、「Devでは動くがProdでは動かない」といった状況になってしまいます。

=== Vite5.1以降では修正入っている

このようなimportの変換はVite5.1以降では修正されています。namespace importをdefault importに変換することは変わりませんが、commonJS形式の場合はオブジェクトに変換するようになっています。そのため、直接関数呼び出しをすることができず、Devモードでもエラーになります。たとえば、importされたDayjsは以下のようになります。@<fn>{PR}

//list[dayjs2][][TypeScript]{
import __vite__cjsImport6_dayjs 
  from "/node_modules/.vite/deps/dayjs.js?v=a45d0170";
const dayjs = (
  (m) => m?.__esModule ? m : {
    ...(typeof m === 'object' && !Array.isArray(m) ? m : {}),
    default: m
  }
)(__vite__cjsImport6_dayjs);
//}

//footnote[PR][https://github.com/vitejs/vite/pull/15619]

== 学び

Viteを使った場合、DevモードとProdモードで挙動が異なる可能性があることが分かりました。ではViteを使うのをやめるべきなのでしょうか？もちろんそんなことはなく、ここから得られる教訓は2つです。

 * <b>{vite dev}だけじゃなくて<b>{vite preview}もしよう
    
    <b>{vite build}でビルドしたものを<b>{vite preview}でビルド成果物を実行することができます。実際のビルド成果物を確認することができるので本番に近いものを試すことができます。今回の問題も<b>{vite preview}しておけばローカルでも気づくことができました。Viteは（現時点の最新5.8では）Devではsbuild、ProdでRollupをビルドツールとして用いているためDevとProdの差分が生まれる可能性は大いにあります。Viteはdevサーバが爆速で立ち上がり非常に便利なので<b>{vite dev}するのみにしてしまいがちでしたので反省です…。
    
 * tsconfigを適切に設定しよう
    
    今回の問題は先述の通りtsconfigを適切に設定しIDEツール上で型エラーにしてあげれば早期に気づくことが可能でした。なるべく型エラーの方に不具合を寄せてあげてビルドツール固有の問題にはしないであげることが重要です（Viteに限った話ではないと思いますが）。
    

Vite5.1以降では修正されているとはいえ、Viteを深く知る良い機会になりました。今後は上記の点に気をつけて開発していきたいと思います。

== 参照

 * https://github.com/vitejs/vite/issues/15542
 * https://qiita.com/eyuta/items/fccebb53d88798c76da5
 * https://www.typescriptlang.org/tsconfig/#esModuleInterop
